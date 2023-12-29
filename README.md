# Regex

Regular Expression Evaluator.

Supported control chars: + ? * . |

The system works by generating a statemachine from a given expression then
the statemachine is used to match expressions. Grouping of character using '(' ')'
is still not allowed (yet). The matching of expression is not greedy and thus
not compatible with other regex engines.

