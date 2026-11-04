//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <optional>
#include <stdexcept>
#include <string>

#include "misc/debug_informations_aware_entity.hpp"


class CompilationError
    : public std::runtime_error
    , public std::optional<DebugInformationsAwareEntity>
{
    public:
        enum class Kind {
            CommandLineError,
            TokenizationError,
            ParsingError,
            IndexingError,
            ValidationError,
            InternalError
        };

        virtual ~CompilationError() = default;
        void display_pretty() const;

        const Kind kind;
        const std::string note;
        using std::runtime_error::what;

    protected:
        CompilationError(
            Kind kind,
            const std::string& message,
            const std::optional<DebugInformationsAwareEntity>& debug_info,
            const std::string& note
        )
            : std::runtime_error(message)
            , std::optional<DebugInformationsAwareEntity>(debug_info)
            , kind(kind)
            , note(note)
        {}
};

struct CommandLineError : public CompilationError {

    virtual ~CommandLineError() = default;
    CommandLineError(const std::string& message)
        : CompilationError(Kind::CommandLineError, message, std::nullopt, "")
    {}
};

struct TokenizationError : public CompilationError {

    virtual ~TokenizationError() = default;
    TokenizationError(
        const std::string& message,
        const DebugInformationsAwareEntity& debug_info,
        const std::string& note = ""
    )
        : CompilationError(Kind::TokenizationError, message, debug_info, note)
    {}
};

struct ParsingError : public CompilationError {

    virtual ~ParsingError() = default;
    ParsingError(
        const std::string& message,
        const DebugInformationsAwareEntity& debug_info,
        const std::string& note = ""
    )
        : CompilationError(Kind::ParsingError, message, debug_info, note)
    {}
};

struct IndexingError : public CompilationError {

    virtual ~IndexingError() = default;
    IndexingError(
        const std::string& message,
        const DebugInformationsAwareEntity& debug_info,
        const std::string& note = ""
    )
        : CompilationError(Kind::IndexingError, message, debug_info, note)
    {}
};

struct ValidationError : public CompilationError {

    virtual ~ValidationError() = default;
    ValidationError(const std::string& message)
        : CompilationError(Kind::ValidationError, message, std::nullopt, "")
    {}

    ValidationError(
        const std::string& message,
        const DebugInformationsAwareEntity& debug_info,
        const std::string& note = ""
    )
        : CompilationError(Kind::ValidationError, message, debug_info, note)
    {}
};

struct InternalError : public CompilationError {

    virtual ~InternalError() = default;
    InternalError(const std::string& message)
        : CompilationError(Kind::InternalError, message, std::nullopt, "")
    {}
};
