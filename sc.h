#pragma once

#include <utility>
#include <type_traits>
#include <array>
#include <cstring>
#include <tuple>

#include "sc_extractors.h"
#include "sc_inserters.h"

static_assert(__cplusplus >= 201103L, "C++11 compliance required.");

namespace sc
{
    template<typename R>
    struct OrderEvaluation
    {
        template<class F, class... Args>
        OrderEvaluation(F&& f, Args&&... args)
            : result(f(std::forward<Args>(args)...))
        { }

        R result;
    };

    template<>
    struct OrderEvaluation<void>
    {
        template<class F, class... Args>
        OrderEvaluation(F&& f, Args&&... args)
        {
            f(std::forward<Args>(args)...);
        }

        int result;
    };

    struct ParserInterface
    {
        virtual void execute(const char* in,
            int max_out = 0, char* out = nullptr) = 0;
    };

    template<typename F, typename S>
    struct Parser;

    template<typename F, typename R, typename... Args>
    struct Parser<F, R(Args...)> : public ParserInterface
    {
        constexpr Parser(const F& f)
            :
            m_function(f)
        { }

        virtual void execute(
            const char* in,
            int max_out = 0, char* out = nullptr)
        {
            if (sizeof...(Args) == 0)
                (void)in;
            auto result = OrderEvaluation<R>{ 
                m_function, extract<Args>(in)... }.result;
            if (max_out > 0 && out != nullptr)
            {
                if (std::is_void<R>::value)
                {
                    out[0] = 0;
                }
                else
                {
                    insert(result, max_out, out);
                }
            }
        }

        protected:
        const F& m_function;
    };

    template<typename S, typename F>
        Parser<F, S> wrap(const F& f)
    {
        return Parser<F, S>(f);
    };

    struct CommandInterface
    {
        virtual const char* name() = 0;
        virtual ParserInterface& parser() = 0;
    };

    template<typename S, typename F>
    class Command : public CommandInterface
    {
        public:
        Command(const char* name, Parser<S, F>&& parser)
            :
            m_name(name),
            m_parser(std::move(parser))
        { }

        virtual const char* name()
        {
            return m_name;
        }

        virtual ParserInterface& parser()
        {
            return m_parser;
        }

        protected:
        const char* m_name;
        Parser<S, F> m_parser;
    };

    template<typename S, typename F>
    Command<S, F> name(const char* command, Parser<S, F>&& parser)
    {
        return Command<S, F>(command, std::move(parser));
    }

    template<typename T>
    class Collection
    {
        public:
        Collection(T&& commands)
            : m_commands(std::move(commands))
        { 
            for (auto& i : m_interfaces)
            {
                i = nullptr;
            }
            register_tuples(*this, m_commands);
        }

        template<typename... Cmds>
        void register_tuples(Collection& c, std::tuple<Cmds...>& cmds)
        {
            register_tuple<sizeof...(Cmds)-1, Cmds...>()(c, cmds);
        }

        template<int i, typename... Cmds>
        struct register_tuple
        {
            void operator()(Collection& c, std::tuple<Cmds...>& cmds)
            {
                c.m_interfaces[i] = &std::get<i>(cmds);
                register_tuple<i-1, Cmds...>()(c, cmds);
            }
        };

        template<typename... Cmds>
        struct register_tuple<0, Cmds...>
        {
            void operator()(Collection& c, std::tuple<Cmds...>& cmds)
            {
                c.m_interfaces[0] = &std::get<0>(cmds);
            }
        };

        bool execute(const char* in, int max_out = 0, char* out = nullptr)
        {
            auto len = command_length(in);

            for (auto& i : m_interfaces)
            {
                if (i != nullptr)
                {
                    if (((int)std::strlen(i->name()) == len)
                        && (std::strncmp(in, i->name(), len) == 0))
                    {
                        i->parser().execute(in + len, max_out, out);
                        return true;
                    }
                }
            }
            return false;
        }

        int command_length(const char* in)
        {
            const char* space = std::strpbrk(in, " \t\r\n");

            if (space == nullptr)
            {
                return std::strlen(in);
            }
            else
            {
                return space - in;
            }
        }

        protected:
        std::array<CommandInterface*, std::tuple_size<T>::value> m_interfaces;
        T m_commands;
    };

    template<typename... Cmds>
    Collection<std::tuple<Cmds...>> collect(Cmds&&... cmds)
    {
        return Collection<std::tuple<Cmds...>>(
            std::make_tuple(std::move(cmds)...));
    }
}

