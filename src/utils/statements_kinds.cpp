
#include "language/statements.hpp"

[[nodiscard]] StatementBody::Kind Statement::statement_kind() const {
    return ptr->statement_kind();
}

[[nodiscard]] StatementBody::Kind VariableDeclaration::statement_kind() const {
    return StatementBody::Kind::variable_declaration;
}

[[nodiscard]] StatementBody::Kind ConstDeclaration::statement_kind() const {
    return StatementBody::Kind::const_declaration;
}

[[nodiscard]] StatementBody::Kind Assignment::statement_kind() const {
    return StatementBody::Kind::assignment;
}

[[nodiscard]] StatementBody::Kind Conditional::statement_kind() const {
    return StatementBody::Kind::conditional;
}

[[nodiscard]] StatementBody::Kind WhileLoop::statement_kind() const {
    return StatementBody::Kind::while_loop;
}

[[nodiscard]] StatementBody::Kind UntilLoop::statement_kind() const {
    return StatementBody::Kind::until_loop;
}

[[nodiscard]] StatementBody::Kind Return::statement_kind() const {
    return StatementBody::Kind::return_statement;
}

[[nodiscard]] StatementBody::Kind Continue::statement_kind() const {
    return StatementBody::Kind::continue_statement;
}

[[nodiscard]] StatementBody::Kind Break::statement_kind() const {
    return StatementBody::Kind::break_statement;
}
