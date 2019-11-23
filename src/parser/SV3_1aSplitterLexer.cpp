
// Generated from SV3_1aSplitterLexer.g4 by ANTLR 4.7.2


#include "SV3_1aSplitterLexer.h"


using namespace antlr4;


SV3_1aSplitterLexer::SV3_1aSplitterLexer(CharStream *input) : Lexer(input) {
  _interpreter = new atn::LexerATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

SV3_1aSplitterLexer::~SV3_1aSplitterLexer() {
  delete _interpreter;
}

std::string SV3_1aSplitterLexer::getGrammarFileName() const {
  return "SV3_1aSplitterLexer.g4";
}

const std::vector<std::string>& SV3_1aSplitterLexer::getRuleNames() const {
  return _ruleNames;
}

const std::vector<std::string>& SV3_1aSplitterLexer::getChannelNames() const {
  return _channelNames;
}

const std::vector<std::string>& SV3_1aSplitterLexer::getModeNames() const {
  return _modeNames;
}

const std::vector<std::string>& SV3_1aSplitterLexer::getTokenNames() const {
  return _tokenNames;
}

dfa::Vocabulary& SV3_1aSplitterLexer::getVocabulary() const {
  return _vocabulary;
}

const std::vector<uint16_t> SV3_1aSplitterLexer::getSerializedATN() const {
  return _serializedATN;
}

const atn::ATN& SV3_1aSplitterLexer::getATN() const {
  return _atn;
}




// Static vars and initialization.
std::vector<dfa::DFA> SV3_1aSplitterLexer::_decisionToDFA;
atn::PredictionContextCache SV3_1aSplitterLexer::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN SV3_1aSplitterLexer::_atn;
std::vector<uint16_t> SV3_1aSplitterLexer::_serializedATN;

std::vector<std::string> SV3_1aSplitterLexer::_ruleNames = {
  u8"One_line_comment", u8"Block_comment", u8"Comment_text", u8"MODULE", 
  u8"ENDMODULE", u8"INTERFACE", u8"ENDINTERFACE", u8"PROGRAM", u8"ENDPROGRAM", 
  u8"PRIMITIVE", u8"ENDPRIMITIVE", u8"PACKAGE", u8"ENDPACKAGE", u8"CHECKER", 
  u8"ENDCHECKER", u8"CONFIG", u8"ENDCONFIG", u8"String", u8"Spaces", u8"WS", 
  u8"TAB", u8"CR", u8"ANY"
};

std::vector<std::string> SV3_1aSplitterLexer::_channelNames = {
  "DEFAULT_TOKEN_CHANNEL", "HIDDEN"
};

std::vector<std::string> SV3_1aSplitterLexer::_modeNames = {
  u8"DEFAULT_MODE"
};

std::vector<std::string> SV3_1aSplitterLexer::_literalNames = {
  "", "", "", u8"'module'", u8"'endmodule'", u8"'interface'", u8"'endinterface'", 
  u8"'program'", u8"'endprogram'", u8"'primivite'", u8"'endprimitive'", 
  u8"'package'", u8"'endpackage'", u8"'checker'", u8"'endchecker'", u8"'config'", 
  u8"'endconfig'"
};

std::vector<std::string> SV3_1aSplitterLexer::_symbolicNames = {
  "", u8"One_line_comment", u8"Block_comment", u8"MODULE", u8"ENDMODULE", 
  u8"INTERFACE", u8"ENDINTERFACE", u8"PROGRAM", u8"ENDPROGRAM", u8"PRIMITIVE", 
  u8"ENDPRIMITIVE", u8"PACKAGE", u8"ENDPACKAGE", u8"CHECKER", u8"ENDCHECKER", 
  u8"CONFIG", u8"ENDCONFIG", u8"String", u8"Spaces", u8"CR", u8"ANY"
};

dfa::Vocabulary SV3_1aSplitterLexer::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> SV3_1aSplitterLexer::_tokenNames;

SV3_1aSplitterLexer::Initializer::Initializer() {
  // This code could be in a static initializer lambda, but VS doesn't allow access to private class members from there.
	for (size_t i = 0; i < _symbolicNames.size(); ++i) {
		std::string name = _vocabulary.getLiteralName(i);
		if (name.empty()) {
			name = _vocabulary.getSymbolicName(i);
		}

		if (name.empty()) {
			_tokenNames.push_back("<INVALID>");
		} else {
      _tokenNames.push_back(name);
    }
	}

  _serializedATN = {
    0x3, 0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964, 
    0x2, 0x16, 0xf9, 0x8, 0x1, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 
    0x4, 0x9, 0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 
    0x7, 0x4, 0x8, 0x9, 0x8, 0x4, 0x9, 0x9, 0x9, 0x4, 0xa, 0x9, 0xa, 0x4, 
    0xb, 0x9, 0xb, 0x4, 0xc, 0x9, 0xc, 0x4, 0xd, 0x9, 0xd, 0x4, 0xe, 0x9, 
    0xe, 0x4, 0xf, 0x9, 0xf, 0x4, 0x10, 0x9, 0x10, 0x4, 0x11, 0x9, 0x11, 
    0x4, 0x12, 0x9, 0x12, 0x4, 0x13, 0x9, 0x13, 0x4, 0x14, 0x9, 0x14, 0x4, 
    0x15, 0x9, 0x15, 0x4, 0x16, 0x9, 0x16, 0x4, 0x17, 0x9, 0x17, 0x4, 0x18, 
    0x9, 0x18, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x5, 0x2, 
    0x37, 0xa, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x7, 
    0x4, 0x45, 0xa, 0x4, 0xc, 0x4, 0xe, 0x4, 0x48, 0xb, 0x4, 0x3, 0x4, 0x7, 
    0x4, 0x4b, 0xa, 0x4, 0xc, 0x4, 0xe, 0x4, 0x4e, 0xb, 0x4, 0x5, 0x4, 0x50, 
    0xa, 0x4, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 
    0x3, 0x5, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 
    0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 
    0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 
    0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 
    0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x9, 
    0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 
    0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 
    0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 
    0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 
    0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 
    0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xd, 
    0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 
    0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 
    0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 
    0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0x10, 0x3, 0x10, 
    0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 
    0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 
    0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 
    0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 
    0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x7, 0x13, 0xdf, 0xa, 0x13, 
    0xc, 0x13, 0xe, 0x13, 0xe2, 0xb, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x14, 
    0x3, 0x14, 0x6, 0x14, 0xe8, 0xa, 0x14, 0xd, 0x14, 0xe, 0x14, 0xe9, 0x3, 
    0x15, 0x6, 0x15, 0xed, 0xa, 0x15, 0xd, 0x15, 0xe, 0x15, 0xee, 0x3, 0x16, 
    0x6, 0x16, 0xf2, 0xa, 0x16, 0xd, 0x16, 0xe, 0x16, 0xf3, 0x3, 0x17, 0x3, 
    0x17, 0x3, 0x18, 0x3, 0x18, 0x3, 0x4c, 0x2, 0x19, 0x3, 0x3, 0x5, 0x4, 
    0x7, 0x2, 0x9, 0x5, 0xb, 0x6, 0xd, 0x7, 0xf, 0x8, 0x11, 0x9, 0x13, 0xa, 
    0x15, 0xb, 0x17, 0xc, 0x19, 0xd, 0x1b, 0xe, 0x1d, 0xf, 0x1f, 0x10, 0x21, 
    0x11, 0x23, 0x12, 0x25, 0x13, 0x27, 0x14, 0x29, 0x2, 0x2b, 0x2, 0x2d, 
    0x15, 0x2f, 0x16, 0x3, 0x2, 0x7, 0x3, 0x2, 0xf, 0xf, 0x6, 0x2, 0xc, 
    0xc, 0xf, 0xf, 0x24, 0x24, 0x5e, 0x5e, 0x3, 0x2, 0x22, 0x22, 0x3, 0x2, 
    0xb, 0xb, 0x4, 0x2, 0xc, 0xc, 0xf, 0xf, 0x2, 0x101, 0x2, 0x3, 0x3, 0x2, 
    0x2, 0x2, 0x2, 0x5, 0x3, 0x2, 0x2, 0x2, 0x2, 0x9, 0x3, 0x2, 0x2, 0x2, 
    0x2, 0xb, 0x3, 0x2, 0x2, 0x2, 0x2, 0xd, 0x3, 0x2, 0x2, 0x2, 0x2, 0xf, 
    0x3, 0x2, 0x2, 0x2, 0x2, 0x11, 0x3, 0x2, 0x2, 0x2, 0x2, 0x13, 0x3, 0x2, 
    0x2, 0x2, 0x2, 0x15, 0x3, 0x2, 0x2, 0x2, 0x2, 0x17, 0x3, 0x2, 0x2, 0x2, 
    0x2, 0x19, 0x3, 0x2, 0x2, 0x2, 0x2, 0x1b, 0x3, 0x2, 0x2, 0x2, 0x2, 0x1d, 
    0x3, 0x2, 0x2, 0x2, 0x2, 0x1f, 0x3, 0x2, 0x2, 0x2, 0x2, 0x21, 0x3, 0x2, 
    0x2, 0x2, 0x2, 0x23, 0x3, 0x2, 0x2, 0x2, 0x2, 0x25, 0x3, 0x2, 0x2, 0x2, 
    0x2, 0x27, 0x3, 0x2, 0x2, 0x2, 0x2, 0x2d, 0x3, 0x2, 0x2, 0x2, 0x2, 0x2f, 
    0x3, 0x2, 0x2, 0x2, 0x3, 0x31, 0x3, 0x2, 0x2, 0x2, 0x5, 0x3a, 0x3, 0x2, 
    0x2, 0x2, 0x7, 0x4f, 0x3, 0x2, 0x2, 0x2, 0x9, 0x51, 0x3, 0x2, 0x2, 0x2, 
    0xb, 0x58, 0x3, 0x2, 0x2, 0x2, 0xd, 0x62, 0x3, 0x2, 0x2, 0x2, 0xf, 0x6c, 
    0x3, 0x2, 0x2, 0x2, 0x11, 0x79, 0x3, 0x2, 0x2, 0x2, 0x13, 0x81, 0x3, 
    0x2, 0x2, 0x2, 0x15, 0x8c, 0x3, 0x2, 0x2, 0x2, 0x17, 0x96, 0x3, 0x2, 
    0x2, 0x2, 0x19, 0xa3, 0x3, 0x2, 0x2, 0x2, 0x1b, 0xab, 0x3, 0x2, 0x2, 
    0x2, 0x1d, 0xb6, 0x3, 0x2, 0x2, 0x2, 0x1f, 0xbe, 0x3, 0x2, 0x2, 0x2, 
    0x21, 0xc9, 0x3, 0x2, 0x2, 0x2, 0x23, 0xd0, 0x3, 0x2, 0x2, 0x2, 0x25, 
    0xda, 0x3, 0x2, 0x2, 0x2, 0x27, 0xe7, 0x3, 0x2, 0x2, 0x2, 0x29, 0xec, 
    0x3, 0x2, 0x2, 0x2, 0x2b, 0xf1, 0x3, 0x2, 0x2, 0x2, 0x2d, 0xf5, 0x3, 
    0x2, 0x2, 0x2, 0x2f, 0xf7, 0x3, 0x2, 0x2, 0x2, 0x31, 0x32, 0x7, 0x31, 
    0x2, 0x2, 0x32, 0x33, 0x7, 0x31, 0x2, 0x2, 0x33, 0x34, 0x3, 0x2, 0x2, 
    0x2, 0x34, 0x36, 0x5, 0x7, 0x4, 0x2, 0x35, 0x37, 0x7, 0xf, 0x2, 0x2, 
    0x36, 0x35, 0x3, 0x2, 0x2, 0x2, 0x36, 0x37, 0x3, 0x2, 0x2, 0x2, 0x37, 
    0x38, 0x3, 0x2, 0x2, 0x2, 0x38, 0x39, 0x7, 0xc, 0x2, 0x2, 0x39, 0x4, 
    0x3, 0x2, 0x2, 0x2, 0x3a, 0x3b, 0x7, 0x31, 0x2, 0x2, 0x3b, 0x3c, 0x7, 
    0x2c, 0x2, 0x2, 0x3c, 0x3d, 0x3, 0x2, 0x2, 0x2, 0x3d, 0x3e, 0x5, 0x7, 
    0x4, 0x2, 0x3e, 0x3f, 0x7, 0x2c, 0x2, 0x2, 0x3f, 0x40, 0x7, 0x31, 0x2, 
    0x2, 0x40, 0x6, 0x3, 0x2, 0x2, 0x2, 0x41, 0x45, 0x5, 0x29, 0x15, 0x2, 
    0x42, 0x45, 0x5, 0x2d, 0x17, 0x2, 0x43, 0x45, 0x5, 0x2b, 0x16, 0x2, 
    0x44, 0x41, 0x3, 0x2, 0x2, 0x2, 0x44, 0x42, 0x3, 0x2, 0x2, 0x2, 0x44, 
    0x43, 0x3, 0x2, 0x2, 0x2, 0x45, 0x48, 0x3, 0x2, 0x2, 0x2, 0x46, 0x44, 
    0x3, 0x2, 0x2, 0x2, 0x46, 0x47, 0x3, 0x2, 0x2, 0x2, 0x47, 0x50, 0x3, 
    0x2, 0x2, 0x2, 0x48, 0x46, 0x3, 0x2, 0x2, 0x2, 0x49, 0x4b, 0xb, 0x2, 
    0x2, 0x2, 0x4a, 0x49, 0x3, 0x2, 0x2, 0x2, 0x4b, 0x4e, 0x3, 0x2, 0x2, 
    0x2, 0x4c, 0x4d, 0x3, 0x2, 0x2, 0x2, 0x4c, 0x4a, 0x3, 0x2, 0x2, 0x2, 
    0x4d, 0x50, 0x3, 0x2, 0x2, 0x2, 0x4e, 0x4c, 0x3, 0x2, 0x2, 0x2, 0x4f, 
    0x46, 0x3, 0x2, 0x2, 0x2, 0x4f, 0x4c, 0x3, 0x2, 0x2, 0x2, 0x50, 0x8, 
    0x3, 0x2, 0x2, 0x2, 0x51, 0x52, 0x7, 0x6f, 0x2, 0x2, 0x52, 0x53, 0x7, 
    0x71, 0x2, 0x2, 0x53, 0x54, 0x7, 0x66, 0x2, 0x2, 0x54, 0x55, 0x7, 0x77, 
    0x2, 0x2, 0x55, 0x56, 0x7, 0x6e, 0x2, 0x2, 0x56, 0x57, 0x7, 0x67, 0x2, 
    0x2, 0x57, 0xa, 0x3, 0x2, 0x2, 0x2, 0x58, 0x59, 0x7, 0x67, 0x2, 0x2, 
    0x59, 0x5a, 0x7, 0x70, 0x2, 0x2, 0x5a, 0x5b, 0x7, 0x66, 0x2, 0x2, 0x5b, 
    0x5c, 0x7, 0x6f, 0x2, 0x2, 0x5c, 0x5d, 0x7, 0x71, 0x2, 0x2, 0x5d, 0x5e, 
    0x7, 0x66, 0x2, 0x2, 0x5e, 0x5f, 0x7, 0x77, 0x2, 0x2, 0x5f, 0x60, 0x7, 
    0x6e, 0x2, 0x2, 0x60, 0x61, 0x7, 0x67, 0x2, 0x2, 0x61, 0xc, 0x3, 0x2, 
    0x2, 0x2, 0x62, 0x63, 0x7, 0x6b, 0x2, 0x2, 0x63, 0x64, 0x7, 0x70, 0x2, 
    0x2, 0x64, 0x65, 0x7, 0x76, 0x2, 0x2, 0x65, 0x66, 0x7, 0x67, 0x2, 0x2, 
    0x66, 0x67, 0x7, 0x74, 0x2, 0x2, 0x67, 0x68, 0x7, 0x68, 0x2, 0x2, 0x68, 
    0x69, 0x7, 0x63, 0x2, 0x2, 0x69, 0x6a, 0x7, 0x65, 0x2, 0x2, 0x6a, 0x6b, 
    0x7, 0x67, 0x2, 0x2, 0x6b, 0xe, 0x3, 0x2, 0x2, 0x2, 0x6c, 0x6d, 0x7, 
    0x67, 0x2, 0x2, 0x6d, 0x6e, 0x7, 0x70, 0x2, 0x2, 0x6e, 0x6f, 0x7, 0x66, 
    0x2, 0x2, 0x6f, 0x70, 0x7, 0x6b, 0x2, 0x2, 0x70, 0x71, 0x7, 0x70, 0x2, 
    0x2, 0x71, 0x72, 0x7, 0x76, 0x2, 0x2, 0x72, 0x73, 0x7, 0x67, 0x2, 0x2, 
    0x73, 0x74, 0x7, 0x74, 0x2, 0x2, 0x74, 0x75, 0x7, 0x68, 0x2, 0x2, 0x75, 
    0x76, 0x7, 0x63, 0x2, 0x2, 0x76, 0x77, 0x7, 0x65, 0x2, 0x2, 0x77, 0x78, 
    0x7, 0x67, 0x2, 0x2, 0x78, 0x10, 0x3, 0x2, 0x2, 0x2, 0x79, 0x7a, 0x7, 
    0x72, 0x2, 0x2, 0x7a, 0x7b, 0x7, 0x74, 0x2, 0x2, 0x7b, 0x7c, 0x7, 0x71, 
    0x2, 0x2, 0x7c, 0x7d, 0x7, 0x69, 0x2, 0x2, 0x7d, 0x7e, 0x7, 0x74, 0x2, 
    0x2, 0x7e, 0x7f, 0x7, 0x63, 0x2, 0x2, 0x7f, 0x80, 0x7, 0x6f, 0x2, 0x2, 
    0x80, 0x12, 0x3, 0x2, 0x2, 0x2, 0x81, 0x82, 0x7, 0x67, 0x2, 0x2, 0x82, 
    0x83, 0x7, 0x70, 0x2, 0x2, 0x83, 0x84, 0x7, 0x66, 0x2, 0x2, 0x84, 0x85, 
    0x7, 0x72, 0x2, 0x2, 0x85, 0x86, 0x7, 0x74, 0x2, 0x2, 0x86, 0x87, 0x7, 
    0x71, 0x2, 0x2, 0x87, 0x88, 0x7, 0x69, 0x2, 0x2, 0x88, 0x89, 0x7, 0x74, 
    0x2, 0x2, 0x89, 0x8a, 0x7, 0x63, 0x2, 0x2, 0x8a, 0x8b, 0x7, 0x6f, 0x2, 
    0x2, 0x8b, 0x14, 0x3, 0x2, 0x2, 0x2, 0x8c, 0x8d, 0x7, 0x72, 0x2, 0x2, 
    0x8d, 0x8e, 0x7, 0x74, 0x2, 0x2, 0x8e, 0x8f, 0x7, 0x6b, 0x2, 0x2, 0x8f, 
    0x90, 0x7, 0x6f, 0x2, 0x2, 0x90, 0x91, 0x7, 0x6b, 0x2, 0x2, 0x91, 0x92, 
    0x7, 0x78, 0x2, 0x2, 0x92, 0x93, 0x7, 0x6b, 0x2, 0x2, 0x93, 0x94, 0x7, 
    0x76, 0x2, 0x2, 0x94, 0x95, 0x7, 0x67, 0x2, 0x2, 0x95, 0x16, 0x3, 0x2, 
    0x2, 0x2, 0x96, 0x97, 0x7, 0x67, 0x2, 0x2, 0x97, 0x98, 0x7, 0x70, 0x2, 
    0x2, 0x98, 0x99, 0x7, 0x66, 0x2, 0x2, 0x99, 0x9a, 0x7, 0x72, 0x2, 0x2, 
    0x9a, 0x9b, 0x7, 0x74, 0x2, 0x2, 0x9b, 0x9c, 0x7, 0x6b, 0x2, 0x2, 0x9c, 
    0x9d, 0x7, 0x6f, 0x2, 0x2, 0x9d, 0x9e, 0x7, 0x6b, 0x2, 0x2, 0x9e, 0x9f, 
    0x7, 0x76, 0x2, 0x2, 0x9f, 0xa0, 0x7, 0x6b, 0x2, 0x2, 0xa0, 0xa1, 0x7, 
    0x78, 0x2, 0x2, 0xa1, 0xa2, 0x7, 0x67, 0x2, 0x2, 0xa2, 0x18, 0x3, 0x2, 
    0x2, 0x2, 0xa3, 0xa4, 0x7, 0x72, 0x2, 0x2, 0xa4, 0xa5, 0x7, 0x63, 0x2, 
    0x2, 0xa5, 0xa6, 0x7, 0x65, 0x2, 0x2, 0xa6, 0xa7, 0x7, 0x6d, 0x2, 0x2, 
    0xa7, 0xa8, 0x7, 0x63, 0x2, 0x2, 0xa8, 0xa9, 0x7, 0x69, 0x2, 0x2, 0xa9, 
    0xaa, 0x7, 0x67, 0x2, 0x2, 0xaa, 0x1a, 0x3, 0x2, 0x2, 0x2, 0xab, 0xac, 
    0x7, 0x67, 0x2, 0x2, 0xac, 0xad, 0x7, 0x70, 0x2, 0x2, 0xad, 0xae, 0x7, 
    0x66, 0x2, 0x2, 0xae, 0xaf, 0x7, 0x72, 0x2, 0x2, 0xaf, 0xb0, 0x7, 0x63, 
    0x2, 0x2, 0xb0, 0xb1, 0x7, 0x65, 0x2, 0x2, 0xb1, 0xb2, 0x7, 0x6d, 0x2, 
    0x2, 0xb2, 0xb3, 0x7, 0x63, 0x2, 0x2, 0xb3, 0xb4, 0x7, 0x69, 0x2, 0x2, 
    0xb4, 0xb5, 0x7, 0x67, 0x2, 0x2, 0xb5, 0x1c, 0x3, 0x2, 0x2, 0x2, 0xb6, 
    0xb7, 0x7, 0x65, 0x2, 0x2, 0xb7, 0xb8, 0x7, 0x6a, 0x2, 0x2, 0xb8, 0xb9, 
    0x7, 0x67, 0x2, 0x2, 0xb9, 0xba, 0x7, 0x65, 0x2, 0x2, 0xba, 0xbb, 0x7, 
    0x6d, 0x2, 0x2, 0xbb, 0xbc, 0x7, 0x67, 0x2, 0x2, 0xbc, 0xbd, 0x7, 0x74, 
    0x2, 0x2, 0xbd, 0x1e, 0x3, 0x2, 0x2, 0x2, 0xbe, 0xbf, 0x7, 0x67, 0x2, 
    0x2, 0xbf, 0xc0, 0x7, 0x70, 0x2, 0x2, 0xc0, 0xc1, 0x7, 0x66, 0x2, 0x2, 
    0xc1, 0xc2, 0x7, 0x65, 0x2, 0x2, 0xc2, 0xc3, 0x7, 0x6a, 0x2, 0x2, 0xc3, 
    0xc4, 0x7, 0x67, 0x2, 0x2, 0xc4, 0xc5, 0x7, 0x65, 0x2, 0x2, 0xc5, 0xc6, 
    0x7, 0x6d, 0x2, 0x2, 0xc6, 0xc7, 0x7, 0x67, 0x2, 0x2, 0xc7, 0xc8, 0x7, 
    0x74, 0x2, 0x2, 0xc8, 0x20, 0x3, 0x2, 0x2, 0x2, 0xc9, 0xca, 0x7, 0x65, 
    0x2, 0x2, 0xca, 0xcb, 0x7, 0x71, 0x2, 0x2, 0xcb, 0xcc, 0x7, 0x70, 0x2, 
    0x2, 0xcc, 0xcd, 0x7, 0x68, 0x2, 0x2, 0xcd, 0xce, 0x7, 0x6b, 0x2, 0x2, 
    0xce, 0xcf, 0x7, 0x69, 0x2, 0x2, 0xcf, 0x22, 0x3, 0x2, 0x2, 0x2, 0xd0, 
    0xd1, 0x7, 0x67, 0x2, 0x2, 0xd1, 0xd2, 0x7, 0x70, 0x2, 0x2, 0xd2, 0xd3, 
    0x7, 0x66, 0x2, 0x2, 0xd3, 0xd4, 0x7, 0x65, 0x2, 0x2, 0xd4, 0xd5, 0x7, 
    0x71, 0x2, 0x2, 0xd5, 0xd6, 0x7, 0x70, 0x2, 0x2, 0xd6, 0xd7, 0x7, 0x68, 
    0x2, 0x2, 0xd7, 0xd8, 0x7, 0x6b, 0x2, 0x2, 0xd8, 0xd9, 0x7, 0x69, 0x2, 
    0x2, 0xd9, 0x24, 0x3, 0x2, 0x2, 0x2, 0xda, 0xe0, 0x7, 0x24, 0x2, 0x2, 
    0xdb, 0xdc, 0x7, 0x5e, 0x2, 0x2, 0xdc, 0xdf, 0xa, 0x2, 0x2, 0x2, 0xdd, 
    0xdf, 0xa, 0x3, 0x2, 0x2, 0xde, 0xdb, 0x3, 0x2, 0x2, 0x2, 0xde, 0xdd, 
    0x3, 0x2, 0x2, 0x2, 0xdf, 0xe2, 0x3, 0x2, 0x2, 0x2, 0xe0, 0xde, 0x3, 
    0x2, 0x2, 0x2, 0xe0, 0xe1, 0x3, 0x2, 0x2, 0x2, 0xe1, 0xe3, 0x3, 0x2, 
    0x2, 0x2, 0xe2, 0xe0, 0x3, 0x2, 0x2, 0x2, 0xe3, 0xe4, 0x7, 0x24, 0x2, 
    0x2, 0xe4, 0x26, 0x3, 0x2, 0x2, 0x2, 0xe5, 0xe8, 0x5, 0x29, 0x15, 0x2, 
    0xe6, 0xe8, 0x5, 0x2b, 0x16, 0x2, 0xe7, 0xe5, 0x3, 0x2, 0x2, 0x2, 0xe7, 
    0xe6, 0x3, 0x2, 0x2, 0x2, 0xe8, 0xe9, 0x3, 0x2, 0x2, 0x2, 0xe9, 0xe7, 
    0x3, 0x2, 0x2, 0x2, 0xe9, 0xea, 0x3, 0x2, 0x2, 0x2, 0xea, 0x28, 0x3, 
    0x2, 0x2, 0x2, 0xeb, 0xed, 0x9, 0x4, 0x2, 0x2, 0xec, 0xeb, 0x3, 0x2, 
    0x2, 0x2, 0xed, 0xee, 0x3, 0x2, 0x2, 0x2, 0xee, 0xec, 0x3, 0x2, 0x2, 
    0x2, 0xee, 0xef, 0x3, 0x2, 0x2, 0x2, 0xef, 0x2a, 0x3, 0x2, 0x2, 0x2, 
    0xf0, 0xf2, 0x9, 0x5, 0x2, 0x2, 0xf1, 0xf0, 0x3, 0x2, 0x2, 0x2, 0xf2, 
    0xf3, 0x3, 0x2, 0x2, 0x2, 0xf3, 0xf1, 0x3, 0x2, 0x2, 0x2, 0xf3, 0xf4, 
    0x3, 0x2, 0x2, 0x2, 0xf4, 0x2c, 0x3, 0x2, 0x2, 0x2, 0xf5, 0xf6, 0x9, 
    0x6, 0x2, 0x2, 0xf6, 0x2e, 0x3, 0x2, 0x2, 0x2, 0xf7, 0xf8, 0xb, 0x2, 
    0x2, 0x2, 0xf8, 0x30, 0x3, 0x2, 0x2, 0x2, 0xe, 0x2, 0x36, 0x44, 0x46, 
    0x4c, 0x4f, 0xde, 0xe0, 0xe7, 0xe9, 0xee, 0xf3, 0x2, 
  };

  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

SV3_1aSplitterLexer::Initializer SV3_1aSplitterLexer::_init;
