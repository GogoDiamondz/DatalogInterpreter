cmake_minimum_required(VERSION 3.20)
project(project_5)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(project_5
        AddAutomaton.cpp
        AddAutomaton.h
        Automaton.h
        ColonAutomaton.cpp
        ColonAutomaton.h
        ColonDashAutomaton.cpp
        ColonDashAutomaton.h
        CommaAutomaton.cpp
        CommaAutomaton.h
        CommentAutomaton.cpp
        CommentAutomaton.h
        Database.cpp
        Database.h
        DatalogProgram.cpp
        DatalogProgram.h
        FactsAutomaton.cpp
        FactsAutomaton.h
        Header.cpp
        Header.h
        IDAutomaton.cpp
        IDAutomaton.h
        Left_ParenAutomaton.cpp
        Left_ParenAutomaton.h
        Lexer.cpp
        Lexer.h
        main.cpp
        MultiplyAutomaton.cpp
        MultiplyAutomaton.h
        Parameter.h
        Parser.cpp
        Parser.h
        PeriodAutomaton.cpp
        PeriodAutomaton.h
        Predicate.cpp
        Predicate.h
        Q_MarkAutomaton.cpp
        Q_MarkAutomaton.h
        QueriesAutomaton.cpp
        QueriesAutomaton.h
        Relation.cpp
        Relation.h
        Right_ParenAutomaton.cpp
        Right_ParenAutomaton.h
        Rule.cpp
        Rule.h
        RulesAutomaton.cpp
        RulesAutomaton.h
        SchemesAutomaton.cpp
        SchemesAutomaton.h
        StringAutomaton.cpp
        StringAutomaton.h
        Token.cpp
        Token.h
        Tuple.cpp
        Tuple.h
        UndefinedCommentAutomaton.cpp
        UndefinedCommentAutomaton.h
        UndefinedStringAutomaton.cpp
        UndefinedStringAutomaton.h Graph.cpp Graph.h)
