//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include "frontend/tokenizer.hpp"
#include "misc/debug_informations_aware_entity.hpp"

#include <string>
#include <exception>

struct CompilationError 
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

        template<Kind kind>
        [[noreturn]]
        static void raise(
            const std::string& message,
            const DebugInformationsAwareEntity& debug_info
        ) {
            static_assert(
                kind != Kind::CommandLineError,
                "Not every type of error can be constructed "
                "with a DebugInformationsAwareEntity "
            );
            throw CompilationError(kind, message, debug_info);
        }

        template<Kind kind>
        [[noreturn]]
        static void raise(
            const std::string& message
        ) {
            static_assert(
                kind == Kind::CommandLineError ||
                kind == Kind::InternalError, 
                "Not every type of error can be constructed " 
                "without a DebugInformationsAwareEntity "
            );
            throw CompilationError(kind, message, std::nullopt);
        }

        virtual ~CompilationError() = default;

        const Kind kind;
        using std::runtime_error::what;
        
        void display_pretty() const;

    private:
        CompilationError(
            Kind kind,
            const std::string& message,
            const std::optional<DebugInformationsAwareEntity>& debug_info
        )
            : std::runtime_error(message)
            , std::optional<DebugInformationsAwareEntity>(debug_info)
            , kind(kind)
        {}
};