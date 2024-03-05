// Generated from ifcc.g4 by ANTLR 4.9.2
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.misc.*;
import org.antlr.v4.runtime.tree.*;
import java.util.List;
import java.util.Iterator;
import java.util.ArrayList;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast"})
public class ifccParser extends Parser {
	static { RuntimeMetaData.checkVersion("4.9.2", RuntimeMetaData.VERSION); }

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		T__0=1, T__1=2, T__2=3, T__3=4, T__4=5, T__5=6, T__6=7, T__7=8, RETURN=9, 
		VARNAME=10, CONST=11, COMMENT=12, DIRECTIVE=13, WS=14;
	public static final int
		RULE_axiom = 0, RULE_prog = 1, RULE_var_declaration = 2, RULE_return_stmt = 3;
	private static String[] makeRuleNames() {
		return new String[] {
			"axiom", "prog", "var_declaration", "return_stmt"
		};
	}
	public static final String[] ruleNames = makeRuleNames();

	private static String[] makeLiteralNames() {
		return new String[] {
			null, "'int'", "'main'", "'('", "')'", "'{'", "'}'", "'='", "';'", "'return'"
		};
	}
	private static final String[] _LITERAL_NAMES = makeLiteralNames();
	private static String[] makeSymbolicNames() {
		return new String[] {
			null, null, null, null, null, null, null, null, null, "RETURN", "VARNAME", 
			"CONST", "COMMENT", "DIRECTIVE", "WS"
		};
	}
	private static final String[] _SYMBOLIC_NAMES = makeSymbolicNames();
	public static final Vocabulary VOCABULARY = new VocabularyImpl(_LITERAL_NAMES, _SYMBOLIC_NAMES);

	/**
	 * @deprecated Use {@link #VOCABULARY} instead.
	 */
	@Deprecated
	public static final String[] tokenNames;
	static {
		tokenNames = new String[_SYMBOLIC_NAMES.length];
		for (int i = 0; i < tokenNames.length; i++) {
			tokenNames[i] = VOCABULARY.getLiteralName(i);
			if (tokenNames[i] == null) {
				tokenNames[i] = VOCABULARY.getSymbolicName(i);
			}

			if (tokenNames[i] == null) {
				tokenNames[i] = "<INVALID>";
			}
		}
	}

	@Override
	@Deprecated
	public String[] getTokenNames() {
		return tokenNames;
	}

	@Override

	public Vocabulary getVocabulary() {
		return VOCABULARY;
	}

	@Override
	public String getGrammarFileName() { return "ifcc.g4"; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public ATN getATN() { return _ATN; }

	public ifccParser(TokenStream input) {
		super(input);
		_interp = new ParserATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}

	public static class AxiomContext extends ParserRuleContext {
		public ProgContext prog() {
			return getRuleContext(ProgContext.class,0);
		}
		public TerminalNode EOF() { return getToken(ifccParser.EOF, 0); }
		public AxiomContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_axiom; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof ifccListener ) ((ifccListener)listener).enterAxiom(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof ifccListener ) ((ifccListener)listener).exitAxiom(this);
		}
	}

	public final AxiomContext axiom() throws RecognitionException {
		AxiomContext _localctx = new AxiomContext(_ctx, getState());
		enterRule(_localctx, 0, RULE_axiom);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(8);
			prog();
			setState(9);
			match(EOF);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ProgContext extends ParserRuleContext {
		public Return_stmtContext return_stmt() {
			return getRuleContext(Return_stmtContext.class,0);
		}
		public List<Var_declarationContext> var_declaration() {
			return getRuleContexts(Var_declarationContext.class);
		}
		public Var_declarationContext var_declaration(int i) {
			return getRuleContext(Var_declarationContext.class,i);
		}
		public ProgContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_prog; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof ifccListener ) ((ifccListener)listener).enterProg(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof ifccListener ) ((ifccListener)listener).exitProg(this);
		}
	}

	public final ProgContext prog() throws RecognitionException {
		ProgContext _localctx = new ProgContext(_ctx, getState());
		enterRule(_localctx, 2, RULE_prog);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(11);
			match(T__0);
			setState(12);
			match(T__1);
			setState(13);
			match(T__2);
			setState(14);
			match(T__3);
			setState(15);
			match(T__4);
			setState(19);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__0) {
				{
				{
				setState(16);
				var_declaration();
				}
				}
				setState(21);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(22);
			return_stmt();
			setState(23);
			match(T__5);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class Var_declarationContext extends ParserRuleContext {
		public List<TerminalNode> VARNAME() { return getTokens(ifccParser.VARNAME); }
		public TerminalNode VARNAME(int i) {
			return getToken(ifccParser.VARNAME, i);
		}
		public TerminalNode CONST() { return getToken(ifccParser.CONST, 0); }
		public Var_declarationContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_var_declaration; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof ifccListener ) ((ifccListener)listener).enterVar_declaration(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof ifccListener ) ((ifccListener)listener).exitVar_declaration(this);
		}
	}

	public final Var_declarationContext var_declaration() throws RecognitionException {
		Var_declarationContext _localctx = new Var_declarationContext(_ctx, getState());
		enterRule(_localctx, 4, RULE_var_declaration);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(25);
			match(T__0);
			setState(26);
			match(VARNAME);
			setState(27);
			match(T__6);
			setState(28);
			_la = _input.LA(1);
			if ( !(_la==VARNAME || _la==CONST) ) {
			_errHandler.recoverInline(this);
			}
			else {
				if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
				_errHandler.reportMatch(this);
				consume();
			}
			setState(29);
			match(T__7);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class Return_stmtContext extends ParserRuleContext {
		public TerminalNode RETURN() { return getToken(ifccParser.RETURN, 0); }
		public TerminalNode CONST() { return getToken(ifccParser.CONST, 0); }
		public Return_stmtContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_return_stmt; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof ifccListener ) ((ifccListener)listener).enterReturn_stmt(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof ifccListener ) ((ifccListener)listener).exitReturn_stmt(this);
		}
	}

	public final Return_stmtContext return_stmt() throws RecognitionException {
		Return_stmtContext _localctx = new Return_stmtContext(_ctx, getState());
		enterRule(_localctx, 6, RULE_return_stmt);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(31);
			match(RETURN);
			setState(32);
			match(CONST);
			setState(33);
			match(T__7);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static final String _serializedATN =
		"\3\u608b\ua72a\u8133\ub9ed\u417c\u3be7\u7786\u5964\3\20&\4\2\t\2\4\3\t"+
		"\3\4\4\t\4\4\5\t\5\3\2\3\2\3\2\3\3\3\3\3\3\3\3\3\3\3\3\7\3\24\n\3\f\3"+
		"\16\3\27\13\3\3\3\3\3\3\3\3\4\3\4\3\4\3\4\3\4\3\4\3\5\3\5\3\5\3\5\3\5"+
		"\2\2\6\2\4\6\b\2\3\3\2\f\r\2\"\2\n\3\2\2\2\4\r\3\2\2\2\6\33\3\2\2\2\b"+
		"!\3\2\2\2\n\13\5\4\3\2\13\f\7\2\2\3\f\3\3\2\2\2\r\16\7\3\2\2\16\17\7\4"+
		"\2\2\17\20\7\5\2\2\20\21\7\6\2\2\21\25\7\7\2\2\22\24\5\6\4\2\23\22\3\2"+
		"\2\2\24\27\3\2\2\2\25\23\3\2\2\2\25\26\3\2\2\2\26\30\3\2\2\2\27\25\3\2"+
		"\2\2\30\31\5\b\5\2\31\32\7\b\2\2\32\5\3\2\2\2\33\34\7\3\2\2\34\35\7\f"+
		"\2\2\35\36\7\t\2\2\36\37\t\2\2\2\37 \7\n\2\2 \7\3\2\2\2!\"\7\13\2\2\""+
		"#\7\r\2\2#$\7\n\2\2$\t\3\2\2\2\3\25";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}