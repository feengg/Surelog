/*
 Copyright 2019 Alain Dargelas
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

/* 
 * File:   DesignElaboration.cpp
 * Author: alain
 * 
 * Created on July 12, 2017, 8:55 PM
 */
#include "../Utils/StringUtils.h"
#include "../SourceCompile/VObjectTypes.h"
#include "../Design/VObject.h"
#include "../Library/Library.h"
#include "../Design/FileContent.h"
#include "../SourceCompile/SymbolTable.h"
#include "../ErrorReporting/Error.h"
#include "../ErrorReporting/Location.h"
#include "../ErrorReporting/Error.h"
#include "../ErrorReporting/ErrorDefinition.h"
#include "../ErrorReporting/ErrorContainer.h"
#include "../Config/ConfigSet.h"
#include "../CommandLine/CommandLineParser.hpp"
#include "../SourceCompile/CompilationUnit.h"
#include "../SourceCompile/PreprocessFile.h"
#include "../SourceCompile/CompileSourceFile.h"
#include "../SourceCompile/ParseFile.h"
#include "../SourceCompile/Compiler.h"
#include "CompileDesign.h"
#include "../Testbench/Property.h"
#include "../Design/Function.h"
#include "../Testbench/ClassDefinition.h"
#include "DesignElaboration.h"
#include <queue>

using namespace SURELOG;

DesignElaboration::DesignElaboration (CompileDesign* compileDesign) : ElaborationStep (compileDesign)
{
  m_moduleDefFactory = NULL;
  m_moduleInstFactory = NULL;
  m_exprBuilder.seterrorReporting (m_compileDesign->getCompiler ()->getErrorContainer (),
                                   m_compileDesign->getCompiler ()->getSymbolTable ());
}

DesignElaboration::~DesignElaboration () { }

bool
DesignElaboration::elaborate ()
{
  createBuiltinPrimitives_();
  setupConfigurations_();
  identifyTopModules_ ();
  elaborateAllModules_ (true);
  elaborateAllModules_ (false);
  reduceUnnamedBlocks_();
  checkElaboration_ ();
  reportElaboration_ ();
  return true;
}

bool
DesignElaboration::setupConfigurations_ ()
{
  ConfigSet* configSet = m_compileDesign->getCompiler ()->getDesign ()->getConfigSet ();
  SymbolTable* st = m_compileDesign->getCompiler ()->getCommandLineParser ()->getSymbolTable ();
  std::vector<Config>& allConfigs = configSet->getAllConfigs ();
  std::vector<SymbolId> selectedConfigIds = m_compileDesign->getCompiler ()->getCommandLineParser ()->getUseConfigs ();
  std::set<std::string> selectedConfigs;
  for (auto confId : selectedConfigIds)
    {
      std::string name = st->getSymbol (confId);
      if (name.find (".") == std::string::npos)
        {
          name = "work@" + name;
        }
      else 
        {
          name = StringUtils::replaceAll (name, ".", "@");
        }
      selectedConfigs.insert (name);
      bool found = false;
      for (auto& config : allConfigs)
        {
          if (config.getName() == name)
            {
              found = true;
              break;
            }
        }
      if (!found)
        {
          Location loc (st->registerSymbol (name));
          Error err (ErrorDefinition::CMD_UNDEFINED_CONFIG, loc);
          m_compileDesign->getCompiler ()->getErrorContainer ()->addError (err);
        }
    }

  std::queue<std::string> configq;
  for (auto& config : allConfigs)
    {
      if (selectedConfigs.find (config.getName ()) != selectedConfigs.end ())
        {
          config.setIsUsed ();
          config.setTopLevel (true);
          configq.push (config.getName ());
        }
    }
  std::set<Config*> configS;
  while (configq.size ())
    {
      std::string configName = configq.front ();
      configq.pop ();
      Config* conf = configSet->getConfig (configName);
      if (conf)
        {
          if (configS.find(conf) != configS.end ())
            {
              continue;
            }
          configS.insert (conf);
          
          conf->setIsUsed ();
          for (auto& usec : conf->getInstanceUseClauses ())
            {
              if (usec.second.getType () == UseClause::UseConfig)
                {
                  std::string confName = usec.second.getName ();
                  configq.push (confName);
                  Config* conf = configSet->getConfig (confName);
                  if (!conf)
                    {
                      FileContent* fC = usec.second.getFileContent ();
                      Location loc (st->registerSymbol(fC->getFileName(usec.second.getNodeId ())), fC->Line(usec.second.getNodeId ()), 0,  st->registerSymbol (confName));
                      Error err (ErrorDefinition::ELAB_UNDEFINED_CONFIG, loc);
                      m_compileDesign->getCompiler ()->getErrorContainer ()->addError (err);
                    }
                  
                }
            }
        }
    }

  std::vector<std::string> unused;
  for (auto& config : allConfigs)
    {
      std::string name = config.getName ();
      FileContent* fC = config.getFileContent ();
      SymbolId fid = st->registerSymbol(fC->getFileName(config.getNodeId ()));
      unsigned int line = fC->Line (config.getNodeId ());
      Location loc (fid, line, 0, st->getId (config.getName ()));
      if (!config.isUsed ())
        {
          Error err (ErrorDefinition::ELAB_CONFIGURATION_IGNORED, loc);
          m_compileDesign->getCompiler ()->getErrorContainer ()->addError (err);
          unused.push_back (name);
        }
      else
        {
          Error err (ErrorDefinition::ELAB_CONFIGURATION_USED, loc);
          m_compileDesign->getCompiler ()->getErrorContainer ()->addError (err);
        }
    }

  // Remove unused configs from set
  for (auto name : unused)
    {
      std::vector<Config>::iterator itr;
      for (itr = allConfigs.begin (); itr != allConfigs.end (); itr++)
        {
          if ((*itr).getName () == name)
            {
              allConfigs.erase (itr);
              break;
            }
        }
    }

  // Create top level module list and recurse configs
  for (auto& config : allConfigs)
    {
      if (config.isTopLevel ())
        {
          std::string lib = config.getDesignLib ();
          std::string top = config.getDesignTop ();
          std::string name = lib + "@" + top;
          m_toplevelConfigModules.insert (name);
          m_instConfig.insert(std::make_pair(name, config));
          m_cellConfig.insert(std::make_pair(name, config));
          
          for (auto& instClause : config.getInstanceUseClauses ())
            {
              m_instUseClause.insert(std::make_pair(lib + "@" + instClause.first, instClause.second));
              if (instClause.second.getType () == UseClause::UseConfig)
                {
                  Config* config = configSet->getConfig (instClause.second.getName ());
                  if (config)
                    {
                      std::set<Config*> configStack;
                      recurseBuildInstanceClause_(lib + "@" + instClause.first, config, configStack);
                    }
                }
            }
          for (auto& cellClause : config.getCellUseClauses ())
            {
              m_cellUseClause.insert(std::make_pair(cellClause.first, cellClause.second));
            }
        }
    }
  return true;
}

void DesignElaboration::recurseBuildInstanceClause_(std::string parentPath, Config* config, std::set<Config*>& configStack)
{
  if (configStack.find(config) != configStack.end()) 
    {      
      return;
    }
  configStack.insert(config);
  
  ConfigSet* configSet = m_compileDesign->getCompiler ()->getDesign ()->getConfigSet ();
  for (auto& useClause : config->getInstanceUseClauses ())
    {
      std::string inst = useClause.first;
      std::string fullPath = parentPath + "." + inst;
      m_instUseClause.insert (std::make_pair (fullPath, useClause.second));
      if (useClause.second.getType () == UseClause::UseConfig)
        {
          Config* config = configSet->getConfig (useClause.second.getName ());
          if (config)
            {
              recurseBuildInstanceClause_ (parentPath + "." + useClause.first, config, configStack);
            }
        }
    }
  for (auto& useClause : config->getCellUseClauses ())
    {
      std::string inst = useClause.first;
      std::string fullPath =  inst;
      m_cellUseClause.insert(std::make_pair(fullPath, useClause.second));
    }
}

bool
DesignElaboration::identifyTopModules_ ()
{
  m_topLevelModules.clear ();
  bool modulePresent = false;
  bool toplevelModuleFound = false;
  SymbolTable* st = m_compileDesign->getCompiler ()->getSymbolTable ();
  auto all_files = m_compileDesign->getCompiler ()->getDesign ()->getAllFileContents ();
  typedef std::multimap<std::string, std::pair<DesignElement*, FileContent*>> ModuleMultiMap;
  ModuleMultiMap all_modules;
  for (auto file : all_files)
    {
      if (m_compileDesign->getCompiler ()->isLibraryFile (file.first))
        continue;
      for (DesignElement& element : file.second->getDesignElements ())
        {
          std::string elemName = st->getSymbol (element.m_name);
          if (element.m_type == DesignElement::Module)
            {
              if (element.m_parent)
                {
                  // This is a nested element
                  continue;
                }
              std::string topname = file.second->getLibrary ()->getName();
              topname += "@" + elemName;
              
              if (!file.second->getParent ())
                {
                  // Files that have parent are splited files (When a module is too large it is splited)
                  all_modules.insert(std::make_pair(topname, std::make_pair(&element, file.second)));          
                }
                           
              modulePresent = true;
              bool used = false;
              for (auto file1 : all_files)
                {
                  if (file1.second->getReferencedObjects ().find (elemName) != file1.second->getReferencedObjects ().end ())
                    {
                      used = true;
                      break;
                    }
                }
              
              if (!used)
                {
                  bool isTop = true;                
                  
                  if (m_toplevelConfigModules.size())
                    {
                      isTop = false;
                      if (m_toplevelConfigModules.find(topname) != m_toplevelConfigModules.end()) 
                        {
                          isTop = true;
                        }
                    }

                  if (isTop)
                    {
                      SymbolId topid = st->registerSymbol (topname);
                      auto itr = m_uniqueTopLevelModules.find (topname);
                      Location loc (st->registerSymbol (file.second->getFileName(element.m_node)),
                                    element.m_line,
                                    0,
                                    topid);
                      if (itr == m_uniqueTopLevelModules.end ())
                        {
                          m_uniqueTopLevelModules.insert (topname);
                          m_topLevelModules.push_back (std::make_pair (topname, file.second));
                          toplevelModuleFound = true;
                          Error err (ErrorDefinition::ELAB_TOP_LEVEL_MODULE, loc);
                          m_compileDesign->getCompiler ()->getErrorContainer ()->addError (err);
                        }                        
                    }
                }
            }
        }
    }
  
  // Check for multiple definition
  std::string prevModuleName = "";
  DesignElement* prevModuleDefinition = NULL;
  FileContent* prevFileContent = NULL;
  for (ModuleMultiMap::iterator itr = all_modules.begin(); itr != all_modules.end(); itr++)   
    {
      std::string    moduleName = (*itr).first;
      DesignElement* moduleDefinition = (*itr).second.first;
      FileContent*   fileContent = (*itr).second.second;
      bool done = false;
      if (moduleName == prevModuleName)
        {
          
          FileContent* fC1 = (*itr).second.second;
          NodeId nodeId1 = moduleDefinition->m_node;
          std::string fileName1 = fC1->getFileName (nodeId1);
          unsigned int line1 = fC1->Line (nodeId1);
          Location loc1 (st->registerSymbol (fileName1), line1, 0, st->registerSymbol (moduleName));

          std::vector<Location> locations;
          
          while (1)
            {

              FileContent* fC2 = prevFileContent;
              NodeId nodeId2 = prevModuleDefinition->m_node;
              std::string fileName2 = fC2->getFileName (nodeId2);
              unsigned int line2 = fC2->Line (nodeId2);
              Location loc2 (st->registerSymbol (fileName2), line2, 0, st->registerSymbol (moduleName));

              if ((fileName1 != fileName2) || (line1 != line2))
                {
                  locations.push_back(loc2);                   
                }
              
              itr++;
              if (itr == all_modules.end())
                {
                  done = true;
                  break;
                }
              else 
                {
                  std::string    nextModuleName = (*itr).first;
                  DesignElement* nextModuleDefinition = (*itr).second.first;
                  FileContent*   nextFileContent = (*itr).second.second;
                  prevModuleName       = nextModuleName;
                  prevModuleDefinition = nextModuleDefinition;
                  prevFileContent      = nextFileContent;
                  if (prevModuleName != moduleName)
                    {
                      moduleName       = prevModuleName;
                      moduleDefinition = prevModuleDefinition;
                      fileContent      = prevFileContent;
                      break;
                    }
                }             
            }
          
          if (locations.size())
            {
              Error err1 (ErrorDefinition::ELAB_MULTIPLY_DEFINED_MODULE, loc1, &locations);
              m_compileDesign->getCompiler ()->getErrorContainer ()->addError (err1);
            }                
        }
      prevModuleName = moduleName;
      prevModuleDefinition = moduleDefinition;
      prevFileContent = fileContent;
      if (done)
        break;      
    }
  
  if (m_topLevelModules.size () > 1)
    {
      Location loc (0);
      Error err (ErrorDefinition::ELAB_MULTIPLE_TOP_LEVEL_MODULES, loc);
      m_compileDesign->getCompiler ()->getErrorContainer ()->addError (err);
    }
  if (modulePresent && (!toplevelModuleFound))
    {
      Location loc (0);
      Error err (ErrorDefinition::ELAB_NO_TOP_LEVEL_MODULE, loc);
      m_compileDesign->getCompiler ()->getErrorContainer ()->addError (err);
    }

  return true;
}

bool
DesignElaboration::createBuiltinPrimitives_ ()
{
  m_moduleDefFactory = new ModuleDefinitionFactory ();
  Design* design = m_compileDesign->getCompiler ()->getDesign ();

  // Register built-in primitives
  for (auto type :{"cmos", "rcmos",
      "bufif0", "bufif1", "notif0", "notif1",
      "nmos", "pmos", "rnmos", "rpmos",
      "and", "or", "nand", "nor", "xor", "xnor",
      "buf", "not",
      "tranif0", "tranif1", "rtranif0", "rtranif1",
      "tran", "rtran", "UnsupportedPrimitive"})
    {
      std::string name = std::string("work@") + type;
      design->addModuleDefinition (name, m_moduleDefFactory->newModuleDefinition (NULL, 0, std::string("work@") + type));
    }
 
  return true;
}

bool
DesignElaboration::elaborateAllModules_ (bool onlyTopLevel)
{
  bool status = true;
  for (auto topmodule : m_topLevelModules)
    {
      if (!elaborateModule_ (topmodule.first, topmodule.second, onlyTopLevel))
        {
          status = false;
        }
    }
  return status;
}

Config* DesignElaboration::getInstConfig(std::string name) {
   Config* config = NULL;
   auto itr = m_instConfig.find(name);
   if (itr != m_instConfig.end())
    {
      config = &(*itr).second;
    }
   return config;
}

Config* DesignElaboration::getCellConfig(std::string name) {
   Config* config = NULL;
   auto itr = m_cellConfig.find(name);
   if (itr != m_cellConfig.end())
    {
      config = &(*itr).second;
    }
   return config;
}


bool
DesignElaboration::elaborateModule_ (std::string moduleName, FileContent* fC, bool onlyTopLevel)
{
  FileContent::NameIdMap& nameIds = fC->getObjectLookup ();
  std::vector<VObjectType> types = {VObjectType::slUdp_instantiation,
    VObjectType::slModule_instantiation,
    VObjectType::slInterface_instantiation,
    VObjectType::slProgram_instantiation};
  std::string libName = fC->getLibrary ()->getName();
  Config* config = getInstConfig(moduleName);
  if (config == NULL)
    config = getCellConfig(moduleName);
  Design* design = m_compileDesign->getCompiler ()->getDesign ();
  if (!m_moduleInstFactory)
    m_moduleInstFactory = new ModuleInstanceFactory ();
  for (auto nameId : nameIds)
    {
      if ((fC->Type (nameId.second) == VObjectType::slModule_declaration) &&
          (moduleName == (libName + "@" + nameId.first)))
        {
          DesignComponent* def = design->getComponentDefinition (moduleName);
          if (onlyTopLevel)
            {
              ModuleInstance* instance = m_moduleInstFactory->newModuleInstance (def, fC, nameId.second, NULL, moduleName, moduleName);
              design->addTopLevelModuleInstance (instance);
            }
          else
            {
              ModuleInstance* instance = design->findInstance (moduleName);
              for (unsigned int i = 0; i < def->getFileContents().size(); i++)
                elaborateInstance_ (def->getFileContents ()[i], def->getNodeIds ()[i], 0, m_moduleInstFactory, instance, config);
            }
          break;
        }
    }
  return true;
}

void
DesignElaboration::recurseInstanceLoop_ (std::vector<int>& from, std::vector<int>& to, std::vector<int>& indexes, unsigned int pos,
                                         DesignComponent* def, FileContent* fC, NodeId subInstanceId, NodeId paramOverride,
                                         ModuleInstanceFactory* factory, ModuleInstance* parent, Config* config,
                                         std::string instanceName, std::string modName, std::vector<ModuleInstance*>& allSubInstances)
{
  if (pos == indexes.size ())
    {
      // This is where the real logic goes.
      // indexes[i] contain the value of the i-th index.
      for (unsigned int i = 0; i < indexes.size (); i++)
        {
          instanceName += std::to_string (indexes[i]);
        }
      ModuleInstance* child = factory->newModuleInstance (def, fC, subInstanceId, parent, instanceName, modName);
      VObjectType type = fC->Type (subInstanceId);
      if (def && (type != VObjectType::slGate_instantiation))
        for (unsigned int i = 0; i < def->getFileContents().size(); i++)
          elaborateInstance_ (def->getFileContents()[i], def->getNodeIds()[i], paramOverride, factory, child, config);
      allSubInstances.push_back (child);

    }
  else
    {
      for (indexes[pos] = from[pos]; indexes[pos] <= to[pos]; indexes[pos]++)
        {
          // Recurse for the next level
          recurseInstanceLoop_ (from, to, indexes, pos + 1, def, fC, subInstanceId, paramOverride,
                                factory, parent, config, instanceName, modName, allSubInstances);
        }
    }
}

void
DesignElaboration::elaborateInstance_ (FileContent* fC, NodeId nodeId, NodeId parentParamOverride, ModuleInstanceFactory* factory, ModuleInstance* parent, Config* config)
{
  if (!parent)
    return;
  std::vector<ModuleInstance*> allSubInstances;
  std::string genBlkBaseName = "genblk";
  unsigned int genBlkIndex = 1;
  bool reuseInstance = false;
  std::string mname;  
  std::vector<VObjectType> types;
  std::vector<std::string> params;

  // Scan for parameters, including DefParams
  collectParams_ (params, fC, nodeId, parent, parentParamOverride);

  //Apply DefParams
  Design* design = m_compileDesign->getCompiler ()->getDesign ();
  for (auto name : params)
    {

      DefParam* defparam = design->getDefParam (parent->getFullPathName () + "." + name);
      if (defparam)
        {
          Value* value = defparam->getValue ();
          if (value)
            {
              parent->setValue (name, value, m_exprBuilder);
              defparam->setUsed ();
            }
        }
    }


  // Scan for regular instances and generate blocks
  types = {VObjectType::slUdp_instantiation,
    VObjectType::slModule_instantiation,
    VObjectType::slInterface_instantiation,
    VObjectType::slProgram_instantiation,
    VObjectType::slGate_instantiation,
    VObjectType::slConditional_generate_construct, // Generate construct are a kind of instantiation
    VObjectType::slGenerate_module_conditional_statement,
    VObjectType::slLoop_generate_construct,
    VObjectType::slGenerate_module_loop_statement,
    VObjectType::slPar_block,
    VObjectType::slSeq_block};


  std::vector<VObjectType> stopPoints = {
    VObjectType::slConditional_generate_construct,
    VObjectType::slGenerate_module_conditional_statement,
    VObjectType::slLoop_generate_construct,
    VObjectType::slGenerate_module_loop_statement,
    VObjectType::slPar_block,
    VObjectType::slSeq_block,
    VObjectType::slModule_declaration
  };

  std::vector<NodeId> subInstances = fC->sl_collect_all (nodeId, types, stopPoints);

  for (auto subInstanceId : subInstances)
    {
      NodeId childId = 0;
      std::string instName;
      std::string modName;
      DesignComponent* def = NULL;
      NodeId paramOverride = 0;
      Config* subConfig = config;
      VObjectType type = fC->Type (subInstanceId);

      if (type == slSeq_block ||
          type == slPar_block)
        {
          NodeId identifierId = fC->Child (subInstanceId);
          if (fC->Name (identifierId))
            instName = fC->SymName (identifierId);
          else
            instName = "UNNAMED";
        }
      else if (type == slConditional_generate_construct)
        {
          NodeId constructId = fC->Child (subInstanceId);
          NodeId condId = fC->Child (constructId);
          NodeId blockId = fC->Sibling(condId);
          NodeId nameId = fC->Child (blockId);
          if (fC->Name (nameId))
            instName = fC->SymName (nameId);
          else
            instName = "UNNAMED";
        }
      else
        {
          NodeId instId = fC->sl_collect (subInstanceId, slName_of_instance);
          NodeId identifierId = 0;
          if (instId != InvalidNodeId)
            {
              identifierId = fC->Child (instId);
              instName = fC->SymName (identifierId);
            }
        }

      // Special module binding for built-in primitives
      if (type == VObjectType::slGate_instantiation)
        {
          VObjectType gatetype = fC->Type (fC->Child (subInstanceId));
          switch (gatetype)
            {
            case VObjectType::slNInpGate_And:
              modName = "work@and";
              break;
            case VObjectType::slNInpGate_Or:
              modName = "work@or";
              break;
            case VObjectType::slNInpGate_Nand:
              modName = "work@nand";
              break;
            case VObjectType::slNInpGate_Nor:
              modName = "work@nor";
              break;
            case VObjectType::slNInpGate_Xor:
              modName = "work@xor";
              break;
            case VObjectType::slNInpGate_Xnor:
              modName = "work@xnor";
              break;
            case VObjectType::slNOutGate_Buf:
              modName = "work@buf";
              break;
            case VObjectType::slNOutGate_Not:
              modName = "work@not";
              break;
              // TODO: Cater for the transistor level
            default:
              modName = "work@UnsupportedPrimitive";
              break;
            }
          
          def = design->getComponentDefinition (modName);
          childId = 0; // = def->getNodeId ();
          ModuleInstance* child = factory->newModuleInstance (def, fC, subInstanceId, parent, instName, modName);
          allSubInstances.push_back (child);
                   
        }
        // Special module binding for generate statements
      else if (type == VObjectType::slConditional_generate_construct ||
               type == VObjectType::slGenerate_module_conditional_statement ||
               type == VObjectType::slLoop_generate_construct ||
               type == VObjectType::slGenerate_module_loop_statement)
        {
          modName = genBlkBaseName + std::to_string (genBlkIndex);

          std::vector<VObjectType> btypes = {VObjectType::slGenerate_module_block,
            VObjectType::slGenerate_block,
            VObjectType::slGenerate_module_named_block};

          std::vector<NodeId> blockIds = fC->sl_collect_all (subInstanceId, btypes, true);
          bool namedBlock = false;
          if (blockIds.size ())
            {
              NodeId blockId = blockIds[0];
              NodeId blockNameId = fC->Child (blockId);
              if (fC->Type (blockNameId) == VObjectType::slStringConst)
                {
                  namedBlock = true;
                  modName = fC->SymName (blockNameId);
                }
            }
          if (!namedBlock)
            genBlkIndex++;
          instName = modName;
          std::string fullName;
          std::string libName = fC->getLibrary()->getName ();
          if (instName == parent->getInstanceName ())
            {
              fullName += parent->getFullPathName ();
              reuseInstance = true;
            }
          else
            {
              fullName += parent->getModuleName () + "." + instName;
            }
          def = design->getComponentDefinition (fullName);
          if (def == NULL)
            {
              def = m_moduleDefFactory->newModuleDefinition (fC, subInstanceId, fullName);
              design->addModuleDefinition (fullName, (ModuleDefinition*) def);
            }

          NodeId conditionId = fC->Child (subInstanceId);

          if (fC->Type (conditionId) == VObjectType::slGenvar_initialization ||
              fC->Type (conditionId) == VObjectType::slGenvar_decl_assignment)
            { // For loop stmt

              // Var init
              NodeId varId = fC->Child (conditionId);
              NodeId constExpr = fC->Sibling (varId);
              Value* initValue = m_exprBuilder.evalExpr (fC, constExpr, parent);
              std::string name = fC->SymName (varId);
              parent->setValue (name, initValue, m_exprBuilder);

              // End-loop test
              NodeId endLoopTest = fC->Sibling (conditionId);

              // Iteration
              NodeId iteration = fC->Sibling (endLoopTest);
              NodeId var = fC->Child (iteration);
              NodeId assignOp = fC->Sibling (var);
              NodeId expr = fC->Sibling (assignOp);
              if (expr == 0)
                { // Unary operator like i++
                  expr = var;
                }
              // Generate block
              NodeId genBlock = fC->Sibling (iteration);

              bool cont = true;
              Value* testCond = m_exprBuilder.evalExpr (fC, endLoopTest, parent);
              cont = testCond->getValueUL ();
              m_exprBuilder.deleteValue (testCond);

              while (cont)
                {
                  Value* currentIndexValue = parent->getValue (name);
                  long currVal = currentIndexValue->getValueUL ();
                  std::string indexedModName = modName + "[" + std::to_string (currVal) + "]";
                  instName = indexedModName;
                  ModuleInstance* child = factory->newModuleInstance (def, fC, genBlock, parent, instName, indexedModName);
                  child->setValue (name, currentIndexValue, m_exprBuilder);
                  elaborateInstance_ (def->getFileContents()[0], genBlock, 0, factory, child, config);
                  allSubInstances.push_back (child);

                  Value* newVal = m_exprBuilder.evalExpr (fC, expr, parent);
                  parent->setValue (name, newVal, m_exprBuilder);
                  Value* testCond = m_exprBuilder.evalExpr (fC, endLoopTest, parent);
                  cont = testCond->getValueUL ();
                  m_exprBuilder.deleteValue (testCond);
                }
              if (allSubInstances.size ())
                {
                  ModuleInstance** children = new ModuleInstance* [allSubInstances.size ()];
                  for (unsigned int index = 0; index < allSubInstances.size (); index++)
                    {
                      children[index] = allSubInstances[index];
                    }
                  parent->addSubInstances (children, allSubInstances.size ());
                }
              continue;

            }
          else
            { // If-Else or Case stmt
              if (fC->Type (conditionId) != VObjectType::slConstant_expression)
                {
                  conditionId = fC->Child (conditionId);
                }
              Value* condValue = m_exprBuilder.evalExpr (fC, conditionId, parent);
              long condVal = condValue->getValueUL ();
              m_exprBuilder.deleteValue (condValue);
              NodeId tmp = fC->Sibling (conditionId);
              if (fC->Type (tmp) == VObjectType::slCase_generate_item)
                { // Case stmt
                  NodeId caseItem = tmp;
                  bool nomatch = true;
                  while (nomatch)
                    {
                      NodeId exprItem = fC->Child (caseItem);
                      if (fC->Type (exprItem) == VObjectType::slGenerate_block) // Default block
                        nomatch = false;
                      while (nomatch)
                        {
                          // Find if one of the case expr matches the case expr
                          if (fC->Type (exprItem) == VObjectType::slConstant_expression)
                            {
                              Value* caseValue = m_exprBuilder.evalExpr (fC, exprItem, parent);
                              long caseVal = caseValue->getValueUL ();
                              m_exprBuilder.deleteValue (caseValue);
                              if (condVal == caseVal)
                                {
                                  nomatch = false;
                                  break;
                                }
                            }
                          else
                            break;
                          exprItem = fC->Sibling (exprItem);
                        }

                      if (nomatch)
                        {
                          // Next case stmt
                          caseItem = fC->Sibling (caseItem);
                          if (!caseItem)
                            break;
                          if (fC->Type (caseItem) != VObjectType::slCase_generate_item)
                            break;
                        }
                      else
                        {
                          // We found a match
                          while (fC->Type (exprItem) == VObjectType::slConstant_expression)
                            exprItem = fC->Sibling (exprItem);
                          childId = exprItem;
                        }
                    }
                }
              else
                { // If-Else stmt
                  if (condVal)
                    { // If branch            
                      if (tmp)
                        childId = tmp;
                      else // There is no If stmt 
                        continue;
                    }
                  else
                    { // Else branch
                      if (tmp)
                        tmp = fC->Sibling (tmp);
                      if (tmp)
                        childId = tmp;
                      else // There is no Else stmt
                        continue;
                    }
                }
            }
          
          
          libName = fC->getLibrary()->getName ();
          fullName = parent->getModuleName () + "." + instName;
          def = design->getComponentDefinition (fullName);
          if (def == NULL)
            {
              def = m_moduleDefFactory->newModuleDefinition (fC, subInstanceId, fullName);
              design->addModuleDefinition (fullName, (ModuleDefinition*) def);
            }

          ModuleInstance* child = factory->newModuleInstance (def, fC, subInstanceId, parent, instName, modName);
          elaborateInstance_ (def->getFileContents()[0], childId, paramOverride, factory, child, config);
          allSubInstances.push_back (child);
          
        }
        // Named blocks  
      else if (type == slSeq_block ||
               type == slPar_block)
        {
          std::string libName = fC->getLibrary()->getName ();
          std::string fullName = parent->getModuleName () + "." + instName;
          
          def = design->getComponentDefinition (fullName);
          if (def == NULL)
            {
              def = m_moduleDefFactory->newModuleDefinition (fC, subInstanceId, fullName);
              design->addModuleDefinition (fullName, (ModuleDefinition*) def);
            }

          ModuleInstance* child = factory->newModuleInstance (def, fC, subInstanceId, parent, instName, modName);
          elaborateInstance_ (def->getFileContents ()[0], subInstanceId, paramOverride, factory, child, config);
          allSubInstances.push_back (child);

        }
        // Regular module binding
      else
        {
          NodeId moduleName = fC->sl_collect (subInstanceId, VObjectType::slStringConst);
          std::string libName = fC->getLibrary ()->getName ();
          mname = fC->SymName (moduleName);

          std::vector<std::string> libs;
          if (config)
            {
              for (auto lib : config->getDefaultLibs ())
                {
                  libs.push_back (lib);
                }
              libs.push_back (libName);
            }
          else
            {
              libs.push_back (libName);
            }
          
          for (auto lib : libs)
            {
              modName = lib + "@" + mname;
              def = design->getComponentDefinition (modName);
              if (def)
                {
                  break;
                }
              else 
                {
                  modName = parent->getDefinition ()->getName () + "::" + mname;
                  def = design->getComponentDefinition (modName);
                  if (def)
                    {
                      break;
                    }
                }
            }

          auto itr = m_cellUseClause.find (mname);
          if (itr != m_cellUseClause.end ())
            {
              UseClause& use = (*itr).second;
              switch (use.getType ())
                {
                case UseClause::UseModule:
                  {
                    std::string name = use.getName ();
                    def = design->getComponentDefinition (name);
                    if (def)
                      use.setUsed ();
                    break;
                  }
                case UseClause::UseLib:
                  {
                    for (auto lib : use.getLibs ())
                      {
                        modName = lib + "@" + mname;
                        def = design->getComponentDefinition (modName);
                        if (def)
                          {
                            use.setUsed ();
                            break;
                          }
                      }
                    break;
                  }
                default:
                  break;
                }
            }
                                  
          if (def)
            childId = def->getNodeIds ()[0];
          
          NodeId tmpId = fC->Sibling (moduleName);
          if (fC->Type (tmpId) == VObjectType::slParameter_value_assignment)
            {
              paramOverride = tmpId;
            }
        

      bool loopDetected = false;
      ModuleInstance* tmp = parent;
      while (tmp)
        {
          if (tmp->getDefinition () == def)
            {
              loopDetected = true;
              break;
            }
          tmp = tmp->getParent ();
        }
      if (loopDetected)
        {
          SymbolTable* st = m_compileDesign->getCompiler ()->getErrorContainer ()->getSymbolTable ();
          Location loc (st->registerSymbol(fC->getFileName(subInstanceId)),
                        fC->Line (subInstanceId),
                        0,
                        st->registerSymbol (modName));
          Location loc2 (st->registerSymbol(tmp->getFileContent ()->getFileName (tmp->getNodeId ())),
                         tmp->getFileContent ()->Line (tmp->getNodeId ()),
                         0);
          Error err (ErrorDefinition::ELAB_INSTANTIATION_LOOP, loc, loc2);
          m_compileDesign->getCompiler ()->getErrorContainer ()->addError (err);
        }
      else
        {
          std::vector<int> from;
          std::vector<int> to;
          std::vector<int> index;

          std::vector<VObjectType> insttypes = {VObjectType::slHierarchical_instance,
            VObjectType::slN_input_gate_instance,
            VObjectType::slN_output_gate_instance,
            VObjectType::slUdp_instance};

          std::vector<NodeId> hierInstIds = fC->sl_collect_all (subInstanceId, insttypes, true);

          NodeId hierInstId = InvalidNodeId;
          if (hierInstIds.size ())
            hierInstId = hierInstIds[0];

          if (hierInstId == InvalidNodeId)
            continue;

          while (hierInstId)
            {
              NodeId instId = fC->sl_collect (hierInstId, slName_of_instance);
              NodeId identifierId = 0;
              if (instId != InvalidNodeId)
                {
                  identifierId = fC->Child (instId);
                  instName = fC->SymName (identifierId);
                }

              auto itr =  m_instUseClause.find(parent->getFullPathName () + "." + instName);
              if (itr != m_instUseClause.end())
                {
                  UseClause& use = (*itr).second;
                  switch (use.getType ())
                    {
                    case UseClause::UseModule:
                    {
                      std::string name = use.getName ();
                      def = design->getComponentDefinition (name);
                      if (def)
                        use.setUsed ();
                      break;
                    }
                    case UseClause::UseLib:
                    {
                      for (auto lib : use.getLibs ())
                        {
                           modName = lib + "@" + mname;
                           def = design->getComponentDefinition (modName);
                           if (def)
                             {
                               use.setUsed ();
                               break;
                             }
                        }
                      break;
                    }
                    case UseClause::UseConfig:
                    {
                      std::string useConfig = use.getName();
                      Config* config = design->getConfigSet ()->getConfig(useConfig);
                      if (config)
                        {
                          subConfig = config;
                          std::string lib = config->getDesignLib ();
                          std::string top = config->getDesignTop ();
                          modName = lib + "@" + top;
                          def = design->getComponentDefinition (modName); 
                          if (def)
                            use.setUsed ();
                          
                        }
                    }
                    default:
                      break;
                    }
                }

              if (def)
                childId = def->getNodeIds ()[0];
              else
                {
                  SymbolTable* st = m_compileDesign->getCompiler ()->getErrorContainer ()->getSymbolTable ();
                  Location loc (st->registerSymbol(fC->getFileName(subInstanceId)), fC->Line (subInstanceId), 0, st->registerSymbol (modName));
                  Error err (ErrorDefinition::ELAB_NO_MODULE_DEFINITION, loc);
                  m_compileDesign->getCompiler ()->getErrorContainer ()->addError (err, false, false);
                }
              
              
              NodeId unpackedDimId = 0;

              if (identifierId)
                unpackedDimId = fC->Sibling (identifierId);

              if (unpackedDimId)
                {
                  // Vector instances
                  while (unpackedDimId)
                    {
                      if (fC->Type (unpackedDimId) == slUnpacked_dimension)
                        {
                          NodeId constantRangeId = fC->Child (unpackedDimId);
                          NodeId leftNode = fC->Child (constantRangeId);
                          NodeId rightNode = fC->Sibling (leftNode);
                          Value* leftVal = m_exprBuilder.evalExpr (fC, leftNode, parent);
                          Value* rightVal = m_exprBuilder.evalExpr (fC, rightNode, parent);
                          unsigned long left = leftVal->getValueUL ();
                          unsigned long right = rightVal->getValueUL ();
                          m_exprBuilder.deleteValue (leftVal);
                          m_exprBuilder.deleteValue (rightVal);
                          if (left < right)
                            {
                              from.push_back (left);
                              to.push_back (right);
                              index.push_back (left);
                            }
                          else
                            {
                              from.push_back (right);
                              to.push_back (left);
                              index.push_back (right);
                            }
                        }
                      unpackedDimId = fC->Sibling (unpackedDimId);
                    }
                  recurseInstanceLoop_ (from, to, index, 0, def, fC, subInstanceId, paramOverride, factory, parent, subConfig, instName, modName, allSubInstances);
                }
              else
                {
                  // Simple instance
                  ModuleInstance* child = NULL;
                  if (reuseInstance)
                    {
                      child = parent;
                      child->setNodeId (subInstanceId);
                    }
                  else
                    {
                      child = factory->newModuleInstance (def, fC, subInstanceId, parent, instName, modName);
                    }
                  if (def && (type != VObjectType::slGate_instantiation))
                    elaborateInstance_ (def->getFileContents ()[0], childId, paramOverride, factory, child, subConfig);

                  if (!reuseInstance)
                    allSubInstances.push_back (child);
                }

              hierInstId = fC->Sibling (hierInstId);
            }

          //std::cout << "INST: " << modName << " " << instName << " " << def << std::endl;
        }
    }
    }
  // Record sub-scopes and sub-instances
  if (allSubInstances.size ())
    {
      ModuleInstance** children = new ModuleInstance* [allSubInstances.size ()];
      for (unsigned int index = 0; index < allSubInstances.size (); index++)
        {
          children[index] = allSubInstances[index];
        }
      parent->addSubInstances (children, allSubInstances.size ());
    }

}

void
DesignElaboration::reportElaboration_ ()
{
  unsigned int nbTopLevelModules = 0;
  unsigned int maxDepth = 0;
  unsigned int numberOfInstances = 0;
  unsigned int numberOfLeafInstances = 0;
  unsigned int nbUndefinedModules = 0;
  unsigned int nbUndefinedInstances = 0;

  m_compileDesign->getCompiler ()->getDesign ()->reportInstanceTreeStats (
                                                                          nbTopLevelModules, maxDepth, numberOfInstances,
                                                                          numberOfLeafInstances, nbUndefinedModules, nbUndefinedInstances);

  SymbolTable* symtable = m_compileDesign->getCompiler ()->getSymbolTable ();

  Location loc1 (symtable->registerSymbol (std::to_string (nbTopLevelModules)));
  Error err1 (ErrorDefinition::ELAB_NB_TOP_LEVEL_MODULES, loc1);
  m_compileDesign->getCompiler ()->getErrorContainer ()->addError (err1);

  Location loc2 (symtable->registerSymbol (std::to_string (maxDepth)));
  Error err2 (ErrorDefinition::ELAB_MAX_INSTANCE_DEPTH, loc2);
  m_compileDesign->getCompiler ()->getErrorContainer ()->addError (err2);

  Location loc3 (symtable->registerSymbol (std::to_string (numberOfInstances)));
  Error err3 (ErrorDefinition::ELAB_NB_INSTANCES, loc3);
  m_compileDesign->getCompiler ()->getErrorContainer ()->addError (err3);

  Location loc4 (symtable->registerSymbol (std::to_string (numberOfLeafInstances)));
  Error err4 (ErrorDefinition::ELAB_NB_LEAF_INSTANCES, loc4);
  m_compileDesign->getCompiler ()->getErrorContainer ()->addError (err4);

  if (nbUndefinedModules)
    {
      Location loc5 (symtable->registerSymbol (std::to_string (nbUndefinedModules)));
      Error err5 (ErrorDefinition::ELAB_NB_UNDEF_MODULES, loc5);
      m_compileDesign->getCompiler ()->getErrorContainer ()->addError (err5);
    }

  if (nbUndefinedInstances)
    {
      Location loc6 (symtable->registerSymbol (std::to_string (nbUndefinedInstances)));
      Error err6 (ErrorDefinition::ELAB_NB_UNDEF_INSTANCES, loc6);
      m_compileDesign->getCompiler ()->getErrorContainer ()->addError (err6);
    }
  CommandLineParser* cl = m_compileDesign->getCompiler ()->getCommandLineParser ();
  if (cl->getDebugInstanceTree () && (!cl->muteStdout ()))
    {
      std::cout << "Instance tree:" << std::endl;
      std::cout << m_compileDesign->getCompiler ()->getDesign ()->reportInstanceTree ();
      std::cout << std::endl;
    }
}

void
DesignElaboration::collectParams_ (std::vector<std::string>& params, FileContent* fC, NodeId nodeId, ModuleInstance* instance, NodeId parentParamOverride)
{
  if (!nodeId)
    return;
  if (!instance)
    return;
  Design* design = m_compileDesign->getCompiler ()->getDesign ();
  SymbolTable* st = m_compileDesign->getCompiler ()->getSymbolTable ();
  ErrorContainer* errors = m_compileDesign->getCompiler ()->getErrorContainer ();
  DesignComponent* module = instance->getDefinition();

  // Parameters imported by package imports 
  std::vector<FileCNodeId> pack_imports;
  for (auto import : fC->getObjects (VObjectType::slPackage_import_item))
    {
      pack_imports.push_back(import);
    }
  for (auto import : module->getObjects (VObjectType::slPackage_import_item))
    {
      pack_imports.push_back(import);
    }
 
  for (auto pack_import : pack_imports)
    {
      NodeId pack_id = pack_import.fC->Child(pack_import.nodeId);
      std::string pack_name = pack_import.fC->SymName (pack_id);      
      Package* def = design->getPackage (pack_name);
      if (def)
        {
          auto& paramSet = def->getObjects (VObjectType::slParam_assignment);
          for (unsigned int i = 0; i < paramSet.size(); i++)
            {
              FileContent* packageFile = paramSet[i].fC;
              NodeId param = paramSet[i].nodeId;
              
              NodeId ident = packageFile->Child (param);
              std::string name = packageFile->SymName (ident);
              Value* value = m_exprBuilder.clone (def->getValues ()[i]);
              instance->setValue (name, value, m_exprBuilder);
              params.push_back(name);             
            }
        }
      else 
        {
          Location loc (st->registerSymbol(pack_import.fC->getFileName (pack_id)), pack_import.fC->Line(pack_id), 0, st->registerSymbol (pack_name));
          Error err (ErrorDefinition::ELAB_UNDEFINED_PACKAGE, loc);
          errors->addError(err);
        }
    }
   
  for (FileCNodeId param : module->getObjects (VObjectType::slParam_assignment))
    {
      NodeId ident = param.fC->Child (param.nodeId);
      std::string name = param.fC->SymName (ident);
      Value* value = m_exprBuilder.evalExpr (param.fC, param.fC->Sibling (ident), instance);
      instance->setValue (name, value, m_exprBuilder);
      params.push_back (name);
    }
  
  std::vector<VObjectType> types;
  // Param overrides
  if (parentParamOverride)
    {
      FileContent* parentFile = instance->getParent()->getDefinition ()->getFileContents()[0];
      types = {VObjectType::slOrdered_parameter_assignment,
        VObjectType::slNamed_parameter_assignment};
      std::vector<NodeId> overrideParams = parentFile->sl_collect_all (parentParamOverride, types);

      unsigned int index = 0;
      for (auto paramAssign : overrideParams)
        {
          NodeId child = parentFile->Child (paramAssign);
          if (parentFile->Type (child) == VObjectType::slStringConst)
            {
              // Named param
              std::string name = parentFile->SymName (child);
              NodeId expr = parentFile->Sibling (child);
              Value* value = m_exprBuilder.evalExpr (parentFile, expr, instance->getParent ());
              instance->setValue (name, value, m_exprBuilder);
            }
          else
            {
              // Index param
              NodeId expr = child;
              Value* value = m_exprBuilder.evalExpr (parentFile, expr, instance->getParent ());
              std::string name = "OUT_OF_RANGE_PARAM_INDEX";
              if (index < params.size()) 
                {
                  name = params[index];
                } 
              else 
                {
                  Location loc (st->registerSymbol(parentFile->getFileName(paramAssign)), parentFile->Line(paramAssign), 0, st->registerSymbol (std::to_string(index)));
                  Error err (ErrorDefinition::ELAB_OUT_OF_RANGE_PARAM_INDEX, loc);
                  errors->addError(err);
                }
              instance->setValue (name, value, m_exprBuilder);
              index++;
            }
        }
    }

  // Defparams
  types = {VObjectType::slDefparam_assignment};
  std::vector<VObjectType> stopPoints = {
    VObjectType::slConditional_generate_construct,
    VObjectType::slGenerate_module_conditional_statement,
    VObjectType::slLoop_generate_construct,
    VObjectType::slGenerate_module_loop_statement,
    VObjectType::slPar_block,
    VObjectType::slSeq_block,
    VObjectType::slModule_declaration
  };

  std::vector<NodeId> defParams = fC->sl_collect_all (nodeId, types, stopPoints);
  for (auto defParam : defParams)
    {

      NodeId hIdent = fC->Child (defParam);
      NodeId var = fC->Child (hIdent);
      NodeId value = fC->Sibling (hIdent);
      std::string fullPath = fC->SymName (var);
      std::string path;
      for (unsigned int i = 0; i < fullPath.size(); i++)
        {
          if (fullPath[i] == '.')
            break;
          path += fullPath[i];
        }
      std::string pathRoot;
      pathRoot = fC->getLibrary ()->getName() + "@";
      pathRoot += path;
      path = fullPath;

      // path refers to a sub instance
      std::string prefix;
      if (design->findInstance (pathRoot))
        {
          std::string p = design->findInstance (pathRoot)->getFullPathName ();
          if (strstr (p.c_str (), "."))
            {
              prefix = instance->getFullPathName () + ".";
            }
          else 
            {
              prefix =  fC->getLibrary ()->getName() + "@";
            }
        }
      else
        {
          prefix = instance->getFullPathName () + ".";
        }
      path = prefix + path;
      Value* val = m_exprBuilder.evalExpr (fC, value, instance);
      design->addDefParam (path, fC, hIdent, val);

    }
}

void
DesignElaboration::checkElaboration_ ()
{
  Design* design = m_compileDesign->getCompiler ()->getDesign ();
  design->checkDefParamUsage ();
  checkConfigurations_();
}

void DesignElaboration::checkConfigurations_() {
  SymbolTable* st = m_compileDesign->getCompiler ()->getSymbolTable ();
  for (auto& pathUseC : m_cellUseClause)
    {
      UseClause& useC = pathUseC.second;
      if (!useC.isUsed ())
        {
          FileContent* fC = useC.getFileContent ();
          Location loc (st->registerSymbol(fC->getFileName(useC.getNodeId ())), fC->Line (useC.getNodeId ()), 0, st->registerSymbol (pathUseC.first));
          Error err (ErrorDefinition::ELAB_USE_CLAUSE_IGNORED, loc);
          m_compileDesign->getCompiler ()->getErrorContainer ()->addError (err);
        }
    }
  for (auto& pathUseC : m_instUseClause)
    {
      UseClause& useC = pathUseC.second;
      if (!useC.isUsed ())
        {
          FileContent* fC = useC.getFileContent ();
          Location loc (st->registerSymbol(fC->getFileName(useC.getNodeId ())), fC->Line (useC.getNodeId ()), 0, st->registerSymbol (pathUseC.first));
          Error err (ErrorDefinition::ELAB_USE_CLAUSE_IGNORED, loc);
          m_compileDesign->getCompiler ()->getErrorContainer ()->addError (err);
        }
    }
}

void
DesignElaboration::reduceUnnamedBlocks_ ()
{
  Design* design = m_compileDesign->getCompiler ()->getDesign ();
  std::queue<ModuleInstance*> queue;
  for (auto instance : design->getTopLevelModuleInstances ())
    {
      queue.push (instance);
    }

  while (queue.size ())
    {
      ModuleInstance* current = queue.front ();
      queue.pop ();
      for (unsigned int i = 0; i < current->getNbChildren (); i++)
        {
          queue.push (current->getChildren (i));
        }
      FileContent* fC = current->getFileContent ();
      NodeId id = current->getNodeId ();
      VObjectType type = fC->Type (id);

      ModuleInstance* parent = current->getParent ();
      if (parent)
        {
          FileContent* fCP = parent->getFileContent ();
          NodeId idP = parent->getNodeId ();
          VObjectType typeP = fCP->Type (idP);

          if ((type == VObjectType::slConditional_generate_construct ||
               type == VObjectType::slGenerate_module_conditional_statement ||
               type == VObjectType::slLoop_generate_construct ||
               type == VObjectType::slGenerate_module_loop_statement)
              &&
              (typeP == VObjectType::slConditional_generate_construct ||
               typeP == VObjectType::slGenerate_module_conditional_statement ||
               typeP == VObjectType::slLoop_generate_construct ||
               typeP == VObjectType::slGenerate_module_loop_statement))
            {
              std::string fullModName = current->getModuleName ();
              fullModName = StringUtils::leaf (fullModName);
              std::string fullModNameP = parent->getModuleName ();
              fullModNameP = StringUtils::leaf (fullModNameP);
              if (strstr (fullModName.c_str (), "genblk"))
                {
                  if (fullModName == fullModNameP)
                    parent->getParent ()->overrideParentChild (parent->getParent (), parent, current);
                }
              else
                {
                  if (strstr (fullModNameP.c_str (), "genblk"))
                    parent->getParent ()->overrideParentChild (parent->getParent (), parent, current);
                }
            }
        }
    }
}
