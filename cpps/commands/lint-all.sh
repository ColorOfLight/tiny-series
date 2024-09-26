# Always perfer references over pointers
CPPLINT_OPTIONS="--filter=-runtime/references"

find ./common -name '*.h' -exec cpplint $CPPLINT_OPTIONS {} +
find ./common -name '*.cpp' -exec cpplint $CPPLINT_OPTIONS {} +
find ./TinyRenderer/include -name '*.h' -exec cpplint $CPPLINT_OPTIONS {} +
find ./TinyRenderer/src -name '*.cpp' -exec cpplint $CPPLINT_OPTIONS {} +
find ./TinyRayTracer/include -name '*.h' -exec cpplint $CPPLINT_OPTIONS {} +
find ./TinyRayTracer/src -name '*.cpp' -exec cpplint $CPPLINT_OPTIONS {} +