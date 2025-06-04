// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.



// First part of user prologue.
#line 4 "dgevalsyn.y"

using namespace std;
#include <iostream>
#include <fstream>

#line 47 "dgevalsyn.tab.cc"


#include "dgevalsyn.tab.hh"


// Unqualified %code blocks.
#line 16 "dgevalsyn.y"

   #include "MyParser.h"
   #define yylex(x) driver->lex(x)

#line 59 "dgevalsyn.tab.cc"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif



// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace yy {
#line 132 "dgevalsyn.tab.cc"

  /// Build a parser object.
  MyParserBase::MyParserBase (MyParser *driver_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      driver (driver_yyarg)
  {}

  MyParserBase::~MyParserBase ()
  {}

  MyParserBase::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/

  // basic_symbol.
  template <typename Base>
  MyParserBase::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value (that.value)
  {}


  /// Constructor for valueless symbols.
  template <typename Base>
  MyParserBase::basic_symbol<Base>::basic_symbol (typename Base::kind_type t)
    : Base (t)
    , value ()
  {}

  template <typename Base>
  MyParserBase::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, YY_RVREF (value_type) v)
    : Base (t)
    , value (YY_MOVE (v))
  {}


  template <typename Base>
  MyParserBase::symbol_kind_type
  MyParserBase::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


  template <typename Base>
  bool
  MyParserBase::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
  MyParserBase::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    value = YY_MOVE (s.value);
  }

  // by_kind.
  MyParserBase::by_kind::by_kind () YY_NOEXCEPT
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  MyParserBase::by_kind::by_kind (by_kind&& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  MyParserBase::by_kind::by_kind (const by_kind& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {}

  MyParserBase::by_kind::by_kind (token_kind_type t) YY_NOEXCEPT
    : kind_ (yytranslate_ (t))
  {}



  void
  MyParserBase::by_kind::clear () YY_NOEXCEPT
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  void
  MyParserBase::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

  MyParserBase::symbol_kind_type
  MyParserBase::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }


  MyParserBase::symbol_kind_type
  MyParserBase::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }



  // by_state.
  MyParserBase::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  MyParserBase::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  MyParserBase::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  MyParserBase::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  MyParserBase::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  MyParserBase::symbol_kind_type
  MyParserBase::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  MyParserBase::stack_symbol_type::stack_symbol_type ()
  {}

  MyParserBase::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.value))
  {
#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  MyParserBase::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.value))
  {
    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  MyParserBase::stack_symbol_type&
  MyParserBase::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    return *this;
  }

  MyParserBase::stack_symbol_type&
  MyParserBase::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  MyParserBase::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);

    // User destructor.
    YY_USE (yysym.kind ());
  }

#if YYDEBUG
  template <typename Base>
  void
  MyParserBase::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " (";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  MyParserBase::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  MyParserBase::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  MyParserBase::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  MyParserBase::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  MyParserBase::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  MyParserBase::debug_level_type
  MyParserBase::debug_level () const
  {
    return yydebug_;
  }

  void
  MyParserBase::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  MyParserBase::state_type
  MyParserBase::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  MyParserBase::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  MyParserBase::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  MyParserBase::operator() ()
  {
    return parse ();
  }

  int
  MyParserBase::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            yyla.kind_ = yytranslate_ (yylex (&yyla.value));
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* If YYLEN is nonzero, implement the default value of the
         action: '$$ = $1'.  Otherwise, use the top of the stack.

         Otherwise, the following line sets YYLHS.VALUE to garbage.
         This behavior is undocumented and Bison users should not rely
         upon it.  */
      if (yylen)
        yylhs.value = yystack_[yylen - 1].value;
      else
        yylhs.value = yystack_[0].value;


      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 3: // statementList: statement
#line 84 "dgevalsyn.y"
                                            {(yylhs.value.statementList)=driver->appendToStatementList((yystack_[0].value.statement));}
#line 589 "dgevalsyn.tab.cc"
    break;

  case 4: // statementList: statementList statement
#line 85 "dgevalsyn.y"
                                            {(yylhs.value.statementList)=driver->appendToStatementList((yystack_[0].value.statement));}
#line 595 "dgevalsyn.tab.cc"
    break;

  case 5: // statement: expStatement
#line 88 "dgevalsyn.y"
                                                        {(yylhs.value.statement)=(yystack_[0].value.expStatement);}
#line 601 "dgevalsyn.tab.cc"
    break;

  case 6: // statement: waitStatement
#line 89 "dgevalsyn.y"
                                              {(yylhs.value.statement)=(yystack_[0].value.waitStatement);}
#line 607 "dgevalsyn.tab.cc"
    break;

  case 7: // expStatement: expression SEMICOLON
#line 93 "dgevalsyn.y"
                                                        {(yylhs.value.expStatement)=driver->createExpStatement((yystack_[1].value.expression));}
#line 613 "dgevalsyn.tab.cc"
    break;

  case 8: // waitStatement: WAIT identifierList THEN expression SEMICOLON
#line 96 "dgevalsyn.y"
                                                                  {(yylhs.value.waitStatement)=driver->createWaitStatement((yystack_[1].value.expression), (yystack_[3].value.identifierList));}
#line 619 "dgevalsyn.tab.cc"
    break;

  case 9: // expression: commaOperand
#line 99 "dgevalsyn.y"
                                                        {(yylhs.value.expression)=(yystack_[0].value.commaOperand);}
#line 625 "dgevalsyn.tab.cc"
    break;

  case 10: // expression: expression COMMA commaOperand
#line 100 "dgevalsyn.y"
                                              {(yylhs.value.expression)=driver->processBILRNode((yystack_[2].value.expression), OP_COMMA, (yystack_[0].value.commaOperand));}
#line 631 "dgevalsyn.tab.cc"
    break;

  case 11: // commaOperand: assignmentSide
#line 103 "dgevalsyn.y"
                                                        {(yylhs.value.commaOperand)=(yystack_[0].value.assignmentSide);}
#line 637 "dgevalsyn.tab.cc"
    break;

  case 12: // commaOperand: commaOperand ASSIGN assignmentSide
#line 104 "dgevalsyn.y"
                                              {(yylhs.value.commaOperand)=driver->processBIRLNode((yystack_[2].value.commaOperand), OP_ASSIGN, (yystack_[0].value.assignmentSide));}
#line 643 "dgevalsyn.tab.cc"
    break;

  case 13: // assignmentSide: booleanOperand ternaryComponent
#line 107 "dgevalsyn.y"
                                                        {(yylhs.value.assignmentSide)=driver->processAssignmentSide((yystack_[1].value.booleanOperand), (yystack_[0].value.ternaryComponent));}
#line 649 "dgevalsyn.tab.cc"
    break;

  case 14: // ternaryComponent: QMARK assignmentSide COLON assignmentSide
#line 110 "dgevalsyn.y"
                                                                 {(yylhs.value.ternaryComponent)=driver->processBILRNode((yystack_[2].value.assignmentSide), OP_ALT, (yystack_[0].value.assignmentSide));}
#line 655 "dgevalsyn.tab.cc"
    break;

  case 15: // ternaryComponent: %empty
#line 111 "dgevalsyn.y"
                  {(yylhs.value.ternaryComponent)=nullptr;}
#line 661 "dgevalsyn.tab.cc"
    break;

  case 16: // booleanOperand: comparisonOperand
#line 114 "dgevalsyn.y"
                                                         {(yylhs.value.booleanOperand)=(yystack_[0].value.comparisonOperand);}
#line 667 "dgevalsyn.tab.cc"
    break;

  case 17: // booleanOperand: booleanOperand booleanOperator comparisonOperand
#line 115 "dgevalsyn.y"
                                                         {(yylhs.value.booleanOperand)=driver->processBILRNode((yystack_[2].value.booleanOperand), (yystack_[1].value.booleanOperator), (yystack_[0].value.comparisonOperand));}
#line 673 "dgevalsyn.tab.cc"
    break;

  case 18: // booleanOperator: BAND
#line 118 "dgevalsyn.y"
                                                       {(yylhs.value.booleanOperator)=OP_BAND;}
#line 679 "dgevalsyn.tab.cc"
    break;

  case 19: // booleanOperator: BOR
#line 119 "dgevalsyn.y"
                                                       {(yylhs.value.booleanOperator)=OP_BOR;}
#line 685 "dgevalsyn.tab.cc"
    break;

  case 20: // comparisonOperand: term
#line 122 "dgevalsyn.y"
                                                       {(yylhs.value.comparisonOperand)=(yystack_[0].value.term);}
#line 691 "dgevalsyn.tab.cc"
    break;

  case 21: // comparisonOperand: comparisonOperand comparisonOperator term
#line 123 "dgevalsyn.y"
                                                       {(yylhs.value.comparisonOperand)=driver->processBILRNode((yystack_[2].value.comparisonOperand), (yystack_[1].value.comparisonOperator), (yystack_[0].value.term));}
#line 697 "dgevalsyn.tab.cc"
    break;

  case 22: // comparisonOperator: EQ
#line 126 "dgevalsyn.y"
                                                       {(yylhs.value.comparisonOperator)=OP_EQ;}
#line 703 "dgevalsyn.tab.cc"
    break;

  case 23: // comparisonOperator: NEQ
#line 127 "dgevalsyn.y"
                                                       {(yylhs.value.comparisonOperator)=OP_NEQ;}
#line 709 "dgevalsyn.tab.cc"
    break;

  case 24: // comparisonOperator: LT
#line 128 "dgevalsyn.y"
                                                       {(yylhs.value.comparisonOperator)=OP_LT;}
#line 715 "dgevalsyn.tab.cc"
    break;

  case 25: // comparisonOperator: LTE
#line 129 "dgevalsyn.y"
                                                       {(yylhs.value.comparisonOperator)=OP_LTE;}
#line 721 "dgevalsyn.tab.cc"
    break;

  case 26: // comparisonOperator: GT
#line 130 "dgevalsyn.y"
                                                       {(yylhs.value.comparisonOperator)=OP_GT;}
#line 727 "dgevalsyn.tab.cc"
    break;

  case 27: // comparisonOperator: GTE
#line 131 "dgevalsyn.y"
                                                       {(yylhs.value.comparisonOperator)=OP_GTE;}
#line 733 "dgevalsyn.tab.cc"
    break;

  case 28: // term: factor
#line 133 "dgevalsyn.y"
                                                       {(yylhs.value.term)=(yystack_[0].value.factor);}
#line 739 "dgevalsyn.tab.cc"
    break;

  case 29: // term: term termOperator factor
#line 134 "dgevalsyn.y"
                                                       {(yylhs.value.term)=driver->processBILRNode((yystack_[2].value.term), (yystack_[1].value.termOperator), (yystack_[0].value.factor));}
#line 745 "dgevalsyn.tab.cc"
    break;

  case 30: // termOperator: PLUS
#line 137 "dgevalsyn.y"
                                                       {(yylhs.value.termOperator)=OP_ADD;}
#line 751 "dgevalsyn.tab.cc"
    break;

  case 31: // termOperator: MINUS
#line 138 "dgevalsyn.y"
                                                       {(yylhs.value.termOperator)=OP_SUB;}
#line 757 "dgevalsyn.tab.cc"
    break;

  case 32: // factor: factorOperand
#line 141 "dgevalsyn.y"
                                                       {(yylhs.value.factor)=(yystack_[0].value.factorOperand);}
#line 763 "dgevalsyn.tab.cc"
    break;

  case 33: // factor: factor factorOperator factorOperand
#line 142 "dgevalsyn.y"
                                                       {(yylhs.value.factor)=driver->processBILRNode((yystack_[2].value.factor), (yystack_[1].value.factorOperator), (yystack_[0].value.factorOperand));}
#line 769 "dgevalsyn.tab.cc"
    break;

  case 34: // factorOperator: MUL
#line 145 "dgevalsyn.y"
                                                                 {(yylhs.value.factorOperator)=OP_MUL;}
#line 775 "dgevalsyn.tab.cc"
    break;

  case 35: // factorOperator: DIV
#line 146 "dgevalsyn.y"
                                                                 {(yylhs.value.factorOperator)=OP_DIV;}
#line 781 "dgevalsyn.tab.cc"
    break;

  case 36: // factorOperand: unaryOperator unaryOperand
#line 149 "dgevalsyn.y"
                                                           {(yylhs.value.factorOperand)=driver->processBinaryOperand((yystack_[1].value.unaryOperator),(yystack_[0].value.unaryOperand));}
#line 787 "dgevalsyn.tab.cc"
    break;

  case 37: // factorOperand: unaryOperand
#line 150 "dgevalsyn.y"
                                                           {(yylhs.value.factorOperand)=(yystack_[0].value.unaryOperand);}
#line 793 "dgevalsyn.tab.cc"
    break;

  case 38: // unaryOperator: MINUS
#line 153 "dgevalsyn.y"
                                                           {(yylhs.value.unaryOperator)=OP_MINUS;}
#line 799 "dgevalsyn.tab.cc"
    break;

  case 39: // unaryOperator: NOT
#line 154 "dgevalsyn.y"
                                                           {(yylhs.value.unaryOperator)=OP_NOT;}
#line 805 "dgevalsyn.tab.cc"
    break;

  case 40: // unaryOperand: unaryOperandBase
#line 157 "dgevalsyn.y"
                                                      {(yylhs.value.unaryOperand)=(yystack_[0].value.unaryOperandBase);}
#line 811 "dgevalsyn.tab.cc"
    break;

  case 41: // unaryOperand: unaryOperand postfixOperator
#line 158 "dgevalsyn.y"
                                                      {(yylhs.value.unaryOperand)=driver->processUnaryOperand((yystack_[1].value.unaryOperand), (yystack_[0].value.postfixOperator));}
#line 817 "dgevalsyn.tab.cc"
    break;

  case 42: // unaryOperandBase: NUM
#line 161 "dgevalsyn.y"
                                                 {(yylhs.value.unaryOperandBase)=driver->processNumericConstant((yystack_[0].value.NUM));}
#line 823 "dgevalsyn.tab.cc"
    break;

  case 43: // unaryOperandBase: TRUE
#line 162 "dgevalsyn.y"
                                                           {(yylhs.value.unaryOperandBase)=driver->processBooleanConstant(true);}
#line 829 "dgevalsyn.tab.cc"
    break;

  case 44: // unaryOperandBase: FALSE
#line 163 "dgevalsyn.y"
                                                           {(yylhs.value.unaryOperandBase)=driver->processBooleanConstant(false);}
#line 835 "dgevalsyn.tab.cc"
    break;

  case 45: // unaryOperandBase: STR
#line 164 "dgevalsyn.y"
                                                           {(yylhs.value.unaryOperandBase)=driver->processStringConstant((yystack_[0].value.STR));}
#line 841 "dgevalsyn.tab.cc"
    break;

  case 46: // unaryOperandBase: OB expression CB
#line 165 "dgevalsyn.y"
                                                           {(yylhs.value.unaryOperandBase)=driver->processArrayLiteral((yystack_[1].value.expression));}
#line 847 "dgevalsyn.tab.cc"
    break;

  case 47: // unaryOperandBase: ID
#line 166 "dgevalsyn.y"
                                                           {(yylhs.value.unaryOperandBase)=driver->processID((yystack_[0].value.ID));}
#line 853 "dgevalsyn.tab.cc"
    break;

  case 48: // unaryOperandBase: LP expression RP
#line 167 "dgevalsyn.y"
                                                           {(yylhs.value.unaryOperandBase)=(yystack_[1].value.expression);}
#line 859 "dgevalsyn.tab.cc"
    break;

  case 49: // actualParameters: expression
#line 170 "dgevalsyn.y"
                                                           {(yylhs.value.actualParameters)=(yystack_[0].value.expression);}
#line 865 "dgevalsyn.tab.cc"
    break;

  case 50: // actualParameters: %empty
#line 171 "dgevalsyn.y"
                                                           {(yylhs.value.actualParameters)=nullptr;}
#line 871 "dgevalsyn.tab.cc"
    break;

  case 51: // postfixOperator: OB expression CB
#line 174 "dgevalsyn.y"
                                                      {(yylhs.value.postfixOperator)=driver->createPostfixNode(OP_AA, (yystack_[1].value.expression));}
#line 877 "dgevalsyn.tab.cc"
    break;

  case 52: // postfixOperator: LP actualParameters RP
#line 175 "dgevalsyn.y"
                                                      {(yylhs.value.postfixOperator)=driver->createPostfixNode(OP_CALL, (yystack_[1].value.actualParameters));}
#line 883 "dgevalsyn.tab.cc"
    break;

  case 53: // identifierList: ID
#line 178 "dgevalsyn.y"
                                                           {(yylhs.value.identifierList)=driver->createIdentifierList((yystack_[0].value.ID));}
#line 889 "dgevalsyn.tab.cc"
    break;

  case 54: // identifierList: identifierList COMMA ID
#line 179 "dgevalsyn.y"
                                                           {(yylhs.value.identifierList)=driver->appendToIdentifierList((yystack_[2].value.identifierList), (yystack_[0].value.ID));}
#line 895 "dgevalsyn.tab.cc"
    break;


#line 899 "dgevalsyn.tab.cc"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        std::string msg = YY_("syntax error");
        error (YY_MOVE (msg));
      }


    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;


      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  MyParserBase::error (const syntax_error& yyexc)
  {
    error (yyexc.what ());
  }

#if YYDEBUG || 0
  const char *
  MyParserBase::symbol_name (symbol_kind_type yysymbol)
  {
    return yytname_[yysymbol];
  }
#endif // #if YYDEBUG || 0









  const signed char MyParserBase::yypact_ninf_ = -35;

  const signed char MyParserBase::yytable_ninf_ = -1;

  const signed char
  MyParserBase::yypact_[] =
  {
       0,     5,   -35,     5,   -35,   -35,   -35,   -35,   -13,   -35,
     -35,    12,     0,   -35,   -35,   -35,    18,     8,   -35,    19,
      35,    59,    61,   -35,    36,     7,   -35,     9,    -5,   -35,
      -6,   -35,   -35,     5,   -35,     5,   -35,   -35,     5,   -35,
       5,   -35,   -35,   -35,   -35,   -35,   -35,     5,   -35,   -35,
       5,   -35,   -35,     5,     7,     5,     5,   -35,   -35,   -35,
      13,     5,     8,   -35,    16,    35,    59,    61,   -35,    11,
      21,    43,   -35,    38,     5,   -35,   -35,   -35,   -35
  };

  const signed char
  MyParserBase::yydefact_[] =
  {
       0,     0,    38,     0,    47,    45,    42,    39,     0,    43,
      44,     0,     2,     3,     5,     6,     0,     9,    11,    15,
      16,    20,    28,    32,     0,    37,    40,     0,     0,    53,
       0,     1,     4,     0,     7,     0,    18,    19,     0,    13,
       0,    24,    25,    26,    27,    22,    23,     0,    30,    31,
       0,    34,    35,     0,    36,     0,    50,    41,    46,    48,
       0,     0,    10,    12,     0,    17,    21,    29,    33,     0,
      49,     0,    54,     0,     0,    51,    52,     8,    14
  };

  const signed char
  MyParserBase::yypgoto_[] =
  {
     -35,   -35,   -35,    47,   -35,   -35,    -1,    29,   -34,   -35,
     -35,   -35,    30,   -35,    24,   -35,    22,   -35,    20,   -35,
      50,   -35,   -35,   -35,   -35
  };

  const signed char
  MyParserBase::yydefgoto_[] =
  {
       0,    11,    12,    13,    14,    15,    16,    17,    18,    39,
      19,    40,    20,    47,    21,    50,    22,    53,    23,    24,
      25,    26,    71,    57,    30
  };

  const signed char
  MyParserBase::yytable_[] =
  {
      27,    63,    28,     1,    64,    59,     2,    29,     1,     3,
      55,     2,    31,    58,     3,    75,    56,    60,    33,    35,
       4,     5,     6,    61,     7,     4,     5,     6,     8,     7,
       9,    10,    33,    72,    33,     9,    10,    36,    37,     1,
      78,    33,    74,    34,    33,     3,    38,    41,    42,    43,
      44,    45,    46,    76,    69,    70,     4,     5,     6,    32,
      73,    33,    62,    77,    48,    49,     9,    10,    51,    52,
      65,    66,    67,    68,    54
  };

  const signed char
  MyParserBase::yycheck_[] =
  {
       1,    35,     3,     3,    38,    10,     6,    20,     3,     9,
       3,     6,     0,     4,     9,     4,     9,    23,    23,    11,
      20,    21,    22,    29,    24,    20,    21,    22,    28,    24,
      30,    31,    23,    20,    23,    30,    31,    18,    19,     3,
      74,    23,    26,    25,    23,     9,    27,    12,    13,    14,
      15,    16,    17,    10,    55,    56,    20,    21,    22,    12,
      61,    23,    33,    25,     5,     6,    30,    31,     7,     8,
      40,    47,    50,    53,    24
  };

  const signed char
  MyParserBase::yystos_[] =
  {
       0,     3,     6,     9,    20,    21,    22,    24,    28,    30,
      31,    33,    34,    35,    36,    37,    38,    39,    40,    42,
      44,    46,    48,    50,    51,    52,    53,    38,    38,    20,
      56,     0,    35,    23,    25,    11,    18,    19,    27,    41,
      43,    12,    13,    14,    15,    16,    17,    45,     5,     6,
      47,     7,     8,    49,    52,     3,     9,    55,     4,    10,
      23,    29,    39,    40,    40,    44,    46,    48,    50,    38,
      38,    54,    20,    38,    26,     4,    10,    25,    40
  };

  const signed char
  MyParserBase::yyr1_[] =
  {
       0,    32,    33,    34,    34,    35,    35,    36,    37,    38,
      38,    39,    39,    40,    41,    41,    42,    42,    43,    43,
      44,    44,    45,    45,    45,    45,    45,    45,    46,    46,
      47,    47,    48,    48,    49,    49,    50,    50,    51,    51,
      52,    52,    53,    53,    53,    53,    53,    53,    53,    54,
      54,    55,    55,    56,    56
  };

  const signed char
  MyParserBase::yyr2_[] =
  {
       0,     2,     1,     1,     2,     1,     1,     2,     5,     1,
       3,     1,     3,     2,     4,     0,     1,     3,     1,     1,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     3,
       1,     1,     1,     3,     1,     1,     2,     1,     1,     1,
       1,     2,     1,     1,     1,     1,     3,     1,     3,     1,
       0,     3,     3,     1,     3
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const MyParserBase::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "OB", "CB", "PLUS",
  "MINUS", "MUL", "DIV", "LP", "RP", "ASSIGN", "LT", "LTE", "GT", "GTE",
  "EQ", "NEQ", "BAND", "BOR", "ID", "STR", "NUM", "COMMA", "NOT",
  "SEMICOLON", "COLON", "QMARK", "WAIT", "THEN", "TRUE", "FALSE",
  "$accept", "dgeval", "statementList", "statement", "expStatement",
  "waitStatement", "expression", "commaOperand", "assignmentSide",
  "ternaryComponent", "booleanOperand", "booleanOperator",
  "comparisonOperand", "comparisonOperator", "term", "termOperator",
  "factor", "factorOperator", "factorOperand", "unaryOperator",
  "unaryOperand", "unaryOperandBase", "actualParameters",
  "postfixOperator", "identifierList", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const unsigned char
  MyParserBase::yyrline_[] =
  {
       0,    81,    81,    84,    85,    88,    89,    93,    96,    99,
     100,   103,   104,   107,   110,   111,   114,   115,   118,   119,
     122,   123,   126,   127,   128,   129,   130,   131,   133,   134,
     137,   138,   141,   142,   145,   146,   149,   150,   153,   154,
     157,   158,   161,   162,   163,   164,   165,   166,   167,   170,
     171,   174,   175,   178,   179
  };

  void
  MyParserBase::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  MyParserBase::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  MyParserBase::symbol_kind_type
  MyParserBase::yytranslate_ (int t) YY_NOEXCEPT
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const signed char
    translate_table[] =
    {
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31
    };
    // Last valid token kind.
    const int code_max = 286;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

} // yy
#line 1307 "dgevalsyn.tab.cc"

#line 181 "dgevalsyn.y"
