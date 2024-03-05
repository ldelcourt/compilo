
// Generated from ifcc.g4 by ANTLR 4.9.2


#include "ifccVisitor.h"

#include "ifccParser.h"


using namespace antlrcpp;
using namespace antlr4;

ifccParser::ifccParser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

ifccParser::~ifccParser() {
  delete _interpreter;
}

std::string ifccParser::getGrammarFileName() const {
  return "ifcc.g4";
}

const std::vector<std::string>& ifccParser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& ifccParser::getVocabulary() const {
  return _vocabulary;
}


//----------------- AxiomContext ------------------------------------------------------------------

ifccParser::AxiomContext::AxiomContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ifccParser::ProgContext* ifccParser::AxiomContext::prog() {
  return getRuleContext<ifccParser::ProgContext>(0);
}

tree::TerminalNode* ifccParser::AxiomContext::EOF() {
  return getToken(ifccParser::EOF, 0);
}


size_t ifccParser::AxiomContext::getRuleIndex() const {
  return ifccParser::RuleAxiom;
}


antlrcpp::Any ifccParser::AxiomContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitAxiom(this);
  else
    return visitor->visitChildren(this);
}

ifccParser::AxiomContext* ifccParser::axiom() {
  AxiomContext *_localctx = _tracker.createInstance<AxiomContext>(_ctx, getState());
  enterRule(_localctx, 0, ifccParser::RuleAxiom);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(8);
    prog();
    setState(9);
    match(ifccParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ProgContext ------------------------------------------------------------------

ifccParser::ProgContext::ProgContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ifccParser::Return_stmtContext* ifccParser::ProgContext::return_stmt() {
  return getRuleContext<ifccParser::Return_stmtContext>(0);
}

std::vector<ifccParser::Var_declarationContext *> ifccParser::ProgContext::var_declaration() {
  return getRuleContexts<ifccParser::Var_declarationContext>();
}

ifccParser::Var_declarationContext* ifccParser::ProgContext::var_declaration(size_t i) {
  return getRuleContext<ifccParser::Var_declarationContext>(i);
}


size_t ifccParser::ProgContext::getRuleIndex() const {
  return ifccParser::RuleProg;
}


antlrcpp::Any ifccParser::ProgContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitProg(this);
  else
    return visitor->visitChildren(this);
}

ifccParser::ProgContext* ifccParser::prog() {
  ProgContext *_localctx = _tracker.createInstance<ProgContext>(_ctx, getState());
  enterRule(_localctx, 2, ifccParser::RuleProg);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(11);
    match(ifccParser::T__0);
    setState(12);
    match(ifccParser::T__1);
    setState(13);
    match(ifccParser::T__2);
    setState(14);
    match(ifccParser::T__3);
    setState(15);
    match(ifccParser::T__4);
    setState(19);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ifccParser::T__0) {
      setState(16);
      var_declaration();
      setState(21);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(22);
    return_stmt();
    setState(23);
    match(ifccParser::T__5);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Var_declarationContext ------------------------------------------------------------------

ifccParser::Var_declarationContext::Var_declarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> ifccParser::Var_declarationContext::VARNAME() {
  return getTokens(ifccParser::VARNAME);
}

tree::TerminalNode* ifccParser::Var_declarationContext::VARNAME(size_t i) {
  return getToken(ifccParser::VARNAME, i);
}

tree::TerminalNode* ifccParser::Var_declarationContext::CONST() {
  return getToken(ifccParser::CONST, 0);
}


size_t ifccParser::Var_declarationContext::getRuleIndex() const {
  return ifccParser::RuleVar_declaration;
}


antlrcpp::Any ifccParser::Var_declarationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitVar_declaration(this);
  else
    return visitor->visitChildren(this);
}

ifccParser::Var_declarationContext* ifccParser::var_declaration() {
  Var_declarationContext *_localctx = _tracker.createInstance<Var_declarationContext>(_ctx, getState());
  enterRule(_localctx, 4, ifccParser::RuleVar_declaration);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(25);
    match(ifccParser::T__0);
    setState(26);
    match(ifccParser::VARNAME);
    setState(27);
    match(ifccParser::T__6);
    setState(28);
    _la = _input->LA(1);
    if (!(_la == ifccParser::VARNAME

    || _la == ifccParser::CONST)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(29);
    match(ifccParser::T__7);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Return_stmtContext ------------------------------------------------------------------

ifccParser::Return_stmtContext::Return_stmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ifccParser::Return_stmtContext::RETURN() {
  return getToken(ifccParser::RETURN, 0);
}

tree::TerminalNode* ifccParser::Return_stmtContext::CONST() {
  return getToken(ifccParser::CONST, 0);
}


size_t ifccParser::Return_stmtContext::getRuleIndex() const {
  return ifccParser::RuleReturn_stmt;
}


antlrcpp::Any ifccParser::Return_stmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitReturn_stmt(this);
  else
    return visitor->visitChildren(this);
}

ifccParser::Return_stmtContext* ifccParser::return_stmt() {
  Return_stmtContext *_localctx = _tracker.createInstance<Return_stmtContext>(_ctx, getState());
  enterRule(_localctx, 6, ifccParser::RuleReturn_stmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(31);
    match(ifccParser::RETURN);
    setState(32);
    match(ifccParser::CONST);
    setState(33);
    match(ifccParser::T__7);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

// Static vars and initialization.
std::vector<dfa::DFA> ifccParser::_decisionToDFA;
atn::PredictionContextCache ifccParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN ifccParser::_atn;
std::vector<uint16_t> ifccParser::_serializedATN;

std::vector<std::string> ifccParser::_ruleNames = {
  "axiom", "prog", "var_declaration", "return_stmt"
};

std::vector<std::string> ifccParser::_literalNames = {
  "", "'int'", "'main'", "'('", "')'", "'{'", "'}'", "'='", "';'", "'return'"
};

std::vector<std::string> ifccParser::_symbolicNames = {
  "", "", "", "", "", "", "", "", "", "RETURN", "VARNAME", "CONST", "COMMENT", 
  "DIRECTIVE", "WS"
};

dfa::Vocabulary ifccParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> ifccParser::_tokenNames;

ifccParser::Initializer::Initializer() {
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

  static const uint16_t serializedATNSegment0[] = {
    0x3, 0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964, 
       0x3, 0x10, 0x26, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 
       0x9, 0x4, 0x4, 0x5, 0x9, 0x5, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 
       0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x7, 0x3, 
       0x14, 0xa, 0x3, 0xc, 0x3, 0xe, 0x3, 0x17, 0xb, 0x3, 0x3, 0x3, 0x3, 
       0x3, 0x3, 0x3, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 
       0x3, 0x4, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x2, 
       0x2, 0x6, 0x2, 0x4, 0x6, 0x8, 0x2, 0x3, 0x3, 0x2, 0xc, 0xd, 0x2, 
       0x22, 0x2, 0xa, 0x3, 0x2, 0x2, 0x2, 0x4, 0xd, 0x3, 0x2, 0x2, 0x2, 
       0x6, 0x1b, 0x3, 0x2, 0x2, 0x2, 0x8, 0x21, 0x3, 0x2, 0x2, 0x2, 0xa, 
       0xb, 0x5, 0x4, 0x3, 0x2, 0xb, 0xc, 0x7, 0x2, 0x2, 0x3, 0xc, 0x3, 
       0x3, 0x2, 0x2, 0x2, 0xd, 0xe, 0x7, 0x3, 0x2, 0x2, 0xe, 0xf, 0x7, 
       0x4, 0x2, 0x2, 0xf, 0x10, 0x7, 0x5, 0x2, 0x2, 0x10, 0x11, 0x7, 0x6, 
       0x2, 0x2, 0x11, 0x15, 0x7, 0x7, 0x2, 0x2, 0x12, 0x14, 0x5, 0x6, 0x4, 
       0x2, 0x13, 0x12, 0x3, 0x2, 0x2, 0x2, 0x14, 0x17, 0x3, 0x2, 0x2, 0x2, 
       0x15, 0x13, 0x3, 0x2, 0x2, 0x2, 0x15, 0x16, 0x3, 0x2, 0x2, 0x2, 0x16, 
       0x18, 0x3, 0x2, 0x2, 0x2, 0x17, 0x15, 0x3, 0x2, 0x2, 0x2, 0x18, 0x19, 
       0x5, 0x8, 0x5, 0x2, 0x19, 0x1a, 0x7, 0x8, 0x2, 0x2, 0x1a, 0x5, 0x3, 
       0x2, 0x2, 0x2, 0x1b, 0x1c, 0x7, 0x3, 0x2, 0x2, 0x1c, 0x1d, 0x7, 0xc, 
       0x2, 0x2, 0x1d, 0x1e, 0x7, 0x9, 0x2, 0x2, 0x1e, 0x1f, 0x9, 0x2, 0x2, 
       0x2, 0x1f, 0x20, 0x7, 0xa, 0x2, 0x2, 0x20, 0x7, 0x3, 0x2, 0x2, 0x2, 
       0x21, 0x22, 0x7, 0xb, 0x2, 0x2, 0x22, 0x23, 0x7, 0xd, 0x2, 0x2, 0x23, 
       0x24, 0x7, 0xa, 0x2, 0x2, 0x24, 0x9, 0x3, 0x2, 0x2, 0x2, 0x3, 0x15, 
  };

  _serializedATN.insert(_serializedATN.end(), serializedATNSegment0,
    serializedATNSegment0 + sizeof(serializedATNSegment0) / sizeof(serializedATNSegment0[0]));


  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

ifccParser::Initializer ifccParser::_init;
