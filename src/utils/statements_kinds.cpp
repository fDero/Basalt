//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "language/statements.hpp"

StatementBody::Kind Statement::statement_kind() const {
    return ptr->statement_kind();
}

StatementBody::Kind VariableDeclaration::statement_kind() const {
    return StatementBody::Kind::variable_declaration;
}

StatementBody::Kind ConstDeclaration::statement_kind() const {
    return StatementBody::Kind::const_declaration;
}

StatementBody::Kind Assignment::statement_kind() const {
    return StatementBody::Kind::assignment;
}

StatementBody::Kind Conditional::statement_kind() const {
    return StatementBody::Kind::conditional;
}

StatementBody::Kind WhileLoop::statement_kind() const {
    return StatementBody::Kind::while_loop;
}

StatementBody::Kind UntilLoop::statement_kind() const {
    return StatementBody::Kind::until_loop;
}

StatementBody::Kind Return::statement_kind() const {
    return StatementBody::Kind::return_statement;
}

StatementBody::Kind Continue::statement_kind() const {
    return StatementBody::Kind::continue_statement;
}

StatementBody::Kind Break::statement_kind() const {
    return StatementBody::Kind::break_statement;
}
