#include "CodeGenVisitor.h"

antlrcpp::Any CodeGenVisitor::visitProg(ifccParser::ProgContext *ctx) 
{
    std::cout<< ".globl _main\n" ;
    std::cout<< " _main: \n" ;

    this->visitChildren(ctx);

    std::cout << "    ret\n";

    return 0;
}


antlrcpp::Any CodeGenVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx)
{
    int index = (int)this->visit(ctx->expression());
    std::cout << "    movl " << index << "(%rbp), %eax\n";

    return 0;
}

antlrcpp::Any CodeGenVisitor::visitVar_affectation(ifccParser::Var_affectationContext *ctx)
{
    std::string declaredVarName = ctx->VARNAME()->getText();
    int declaredVarIndex = symbolTable.getSymbolIndex(declaredVarName);

    int assignedVarIndex = (int)this->visit(ctx->expression());
    std::cout << "    movl " << assignedVarIndex << "(%rbp), %eax\n";
    std::cout << "    movl %eax, " << declaredVarIndex << "(%rbp)\n";

    return 0;
}

antlrcpp::Any CodeGenVisitor::visitVar_initialisation(ifccParser::Var_initialisationContext *ctx)
{
    std::string declaredVarName = ctx->VARNAME()->getText();
    int declaredVarIndex = symbolTable.getSymbolIndex(declaredVarName);

    int assignedVarIndex = (int)(this->visit(ctx->expression()));
    std::cout << "    movl " << assignedVarIndex << "(%rbp), %eax\n";
    std::cout << "    movl %eax, " << declaredVarIndex << "(%rbp)\n";

    return 0;
}

antlrcpp::Any CodeGenVisitor::visitAddsub(ifccParser::AddsubContext *ctx) // expr '+/-' expr
{
    int leftHandSide = (int)this->visit(ctx->expression(0));
    int rightHandSide = (int)this->visit(ctx->expression(1));

    int tmpVarIndex = symbolTable.autoAddSymbol();
    std::cout << "    movl " << leftHandSide << "(%rbp), %eax\n";
    std::cout << "    movl " << rightHandSide << "(%rbp), %ecx\n";

    if(ctx->OP->getText() == "+") {
        std::cout << "    addl %ecx, %eax\n";
    }
    else if (ctx->OP->getText() == "-") {
        std::cout << "    subl %ecx, %eax\n";
    }
    std::cout << "    movl %eax, " << tmpVarIndex << "(%rbp)\n";
    return tmpVarIndex;
}

antlrcpp::Any CodeGenVisitor::visitMultdiv(ifccParser::MultdivContext *ctx) // expr '*/' expr
{
    int leftHandSide = (int)this->visit(ctx->expression(0));
    int rightHandSide = (int)this->visit(ctx->expression(1));

    int tmpVarIndex = symbolTable.autoAddSymbol();
    std::cout << "    movl " << leftHandSide << "(%rbp), %eax\n";
    std::cout << "    movl " << rightHandSide << "(%rbp), %ecx\n";

    if(ctx->OP->getText() == "*") {
        std::cout << "    imull %ecx, %eax\n";
    }
    else if (ctx->OP->getText() == "/") {
        std::cout << "    cltd\n";
        std::cout << "    idivl %ecx\n";
    }
    std::cout << "    movl %eax, " << tmpVarIndex << "(%rbp)\n";
    return tmpVarIndex;
}

antlrcpp::Any CodeGenVisitor::visitPar(ifccParser::ParContext *ctx) // '(' expr ')'
{
    return (int)this->visit(ctx->expression());
}

antlrcpp::Any CodeGenVisitor::visitVar(ifccParser::VarContext *ctx) // VARNAME
{
    std::string varName = ctx->VARNAME()->getText();
    int varIndex = symbolTable.getSymbolIndex(varName);
    return varIndex;
}

antlrcpp::Any CodeGenVisitor::visitConst(ifccParser::ConstContext *ctx) // CONST
{
    std::string constValue = ctx->CONST()->getText();
    int tmpVarIndex = symbolTable.autoAddSymbol();
    std::cout << "    movl $" << constValue << ", " << tmpVarIndex << "(%rbp)\n";
    return tmpVarIndex;
}
