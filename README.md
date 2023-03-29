build (from project root):<br>
<code>cmake -DCMAKE_BUILD_TYPE=Release -DENABLE_TESTS=on -G "Unix Makefiles" -S . -B ./build && cmake --build ./build -j4</code><br>
run example (main.cpp):<br>
<code>./build/splitWork</code><br>
run tests:<br>
<code>cd ./build/tests && ctest</code>