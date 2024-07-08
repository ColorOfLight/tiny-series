# Always perfer references over pointers
CPPLINT_OPTIONS="--filter=-runtime/references"

find ./TinyRenderer/include -name '*.h' -exec cpplint $CPPLINT_OPTIONS {} +
find ./TinyRenderer/src -name '*.cpp' -exec cpplint $CPPLINT_OPTIONS {} +
