#!/usr/bin/tclsh

# Copyright 2019 Alain Dargelas
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


set oid [open "SourceCompile/SV3_1aTreeShapeListener.h" "w"]

# Read .cpp hand written methods
set cid [open "SourceCompile/SV3_1aTreeShapeListener.cpp"]
set cpp_spec "[read $cid]"
close $cid
set lines [split $cpp_spec "\n"]
foreach line $lines {
    if {[regexp {::(enter[a-zA-Z0-9_]+)} $line tmp method] || [regexp {::(exit[a-zA-Z0-9_]+)} $line tmp method] || [regexp {::(visit[a-zA-Z0-9_]+)} $line tmp method]} {
	set CUSTOM_METHOD($method) 1
    }
}

# Read Listener definition
set hid [open "parser/SV3_1aParserBaseListener.h"]
set h_spec "[read $hid]"
close $hid
set lines [split $h_spec "\n"]

puts $oid "/*"
puts $oid " Copyright 2019 Alain Dargelas"
puts $oid " Licensed under the Apache License, Version 2.0 (the \"License\");"
puts $oid " you may not use this file except in compliance with the License."
puts $oid " You may obtain a copy of the License at"
puts $oid ""
puts $oid "    http://www.apache.org/licenses/LICENSE-2.0"
puts $oid ""
puts $oid " Unless required by applicable law or agreed to in writing, software"
puts $oid " distributed under the License is distributed on an \"AS IS\" BASIS,"
puts $oid " WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied."
puts $oid " See the License for the specific language governing permissions and"
puts $oid " limitations under the License."
puts $oid " */"
puts $oid ""
puts $oid "/*  This file is automatically generated by generate_parser_listener.tcl"
puts $oid " *  DO NOT EDIT THIS FILE."
puts $oid " *  If a method needs custom operator, write the method in SV3_1aTreeShapeListener.cpp" 
puts $oid "*/"
puts $oid ""
puts $oid "/* "
puts $oid " * File:   SV3_1aTreeShapeListener.h"
puts $oid " * Author: alain"
puts $oid " *"
puts $oid " * Created on April 16, 2017, 8:28 PM"
puts $oid " */"
puts $oid ""
puts $oid "#ifndef SV3_1ATREESHAPELISTENER_H"
puts $oid "#define SV3_1ATREESHAPELISTENER_H"
puts $oid "#include <stack>"
puts $oid "#include <map>"
puts $oid "#include <unordered_map>"
puts $oid "#include \"../Utils/ParseUtils.h\""
puts $oid "#include \"../SourceCompile/SymbolTable.h\""
puts $oid "#include \"../Design/TimeInfo.h\""
puts $oid "#include \"../Design/DesignElement.h\""
puts $oid "#include \"../Design/FileContent.h\""
puts $oid "#include \"ParseFile.h\""
puts $oid "#include \"CompilationUnit.h\""
puts $oid "#include \"CompileSourceFile.h\""
puts $oid "#include \"SV3_1aTreeShapeHelper.h\""
puts $oid ""
puts $oid "namespace SURELOG {"
puts $oid ""
puts $oid "    class SV3_1aTreeShapeListener : public SV3_1aParserBaseListener, public SV3_1aTreeShapeHelper  {"
puts $oid "    private:"
puts $oid " " 
puts $oid "    public:"
puts $oid "        SV3_1aTreeShapeListener(ParseFile* pf, antlr4::CommonTokenStream* tokens, unsigned int lineOffset);"
puts $oid " "       
puts $oid "        ~SV3_1aTreeShapeListener();"
puts $oid " "       

foreach line $lines {
    if {[regexp {virtual void ([a-zA-Z0-9_]+)} $line tmp method]} {
	if [info exist CUSTOM_METHOD($method)] {
	    regsub {\{ \}} $line ";" line
	    puts $oid $line
	} else {
	    if [regsub "exit" $method ""  method] {
		regsub "enter" $method "" method
		regsub "visit" $method ""  method
		set code "addVObject (ctx, VObjectType::sl${method});"
		set TYPES(sl${method}) 1
		regsub {\{ \}} $line "{ $code }" line
		regsub {/\*ctx\*/} $line "ctx" line
	    }
	    puts $oid $line
	}
    }
}

puts $oid "    };"
puts $oid "};"
puts $oid ""
puts $oid "#endif /* SV3_1ATREESHAPELISTENER_H */"
puts $oid ""
close $oid
set TYPES(slNoType) 1
set TYPES(slComments) 1
set TYPES(slModule) 1
# both Module_declaration and Interface_declaration enter and exit rules are in SV3_1aTreeShapeListener.cpp file
set TYPES(slModule_declaration) 1 
set TYPES(slInterface_declaration) 1
# Class_type exit is in SV3_1aTreeShapeListener.cpp file
set TYPES(slClass_type) 1
set TYPES(slHierarchical_identifier) 1
set TYPES(slModuleInstance) 1
set TYPES(slPrimitive) 1
set TYPES(slPrimitiveInstance) 1
set TYPES(slInterface) 1
set TYPES(slProgram) 1
set TYPES(slPackage) 1
set TYPES(slChecker) 1
set TYPES(slClass) 1
set TYPES(slPortInst) 1
set TYPES(slConstSelect) 1
set TYPES(slIntConst) 1
set TYPES(slRealConst) 1
set TYPES(slStringConst) 1
set TYPES(slStringLiteral) 1
set TYPES(slConstantSelect) 1
set TYPES(slThis) 1
set TYPES(slGenericElementType) 1
set TYPES(sl0) 1
set TYPES(sl1) 1
set TYPES(slX) 1
set TYPES(slZ) 1

set oid [open "SourceCompile/VObjectTypes.h" "w"]
puts $oid ""
puts $oid "// This file is automatically generated by generate_parser_listener.tcl"
puts $oid "// DO NOT EDIT"
puts $oid ""
puts $oid "#ifndef VOBJECTTYPES_H"
puts $oid "#define VOBJECTTYPES_H"

puts $oid "typedef enum {"
set id 0
foreach type [lsort -dictionary [array names TYPES]] {
    puts $oid "      $type = $id,"
    incr id
}
puts $oid "} VObjectType;"
puts $oid ""
puts $oid "#endif /* VOBJECTTYPES_H */"
puts $oid ""

close $oid


set oid [open "SourceCompile/VObjectTypes.cpp" "w"]
puts $oid ""
puts $oid "#include <string>"
puts $oid "#include \"../Design/VObject.h\""
puts $oid "// This file is automatically generated by generate_parser_listener.tcl"
puts $oid "// DO NOT EDIT"
puts $oid ""
puts $oid "using namespace SURELOG;"
puts $oid ""
puts $oid "std::string VObject::getTypeName(unsigned short type) {"
puts $oid "    std::string text;"
puts $oid "    switch (type) {"
foreach type [lsort -dictionary [array names TYPES]] {
    puts $oid "    case $type:"
    puts $oid "      text = \"$type\";"
    puts $oid "      break;"
}
puts $oid "    };"
puts $oid "    return text;"
puts $oid "}"
puts $oid ""
close $oid

set oid [open "API/vobjecttypes_py.h" "w"]
puts $oid ""
puts $oid "std::vector<std::string> slapi_types = {"
puts $oid "\"\# This file is automatically generated by generate_parser_listener.tcl\\n\""
puts $oid "\"\# DO NOT EDIT\\n\""

set id 0
foreach type [lsort -dictionary [array names TYPES]] {
    puts $oid "\"$type = $id;\\n\""
    incr id
}
puts $oid "};"

close $oid

flush stdout

