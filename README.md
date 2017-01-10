# stringcommander
Simplified (?) debug access to embedded projects.

### how-to
```c++
#include "sc.h"

    // create a collection of named commands, wrapping existing functions:
    auto c = sc::collect(
        sc::name("ctime",   sc::wrap<const char*()>(compiletime)),
        sc::name("add",     sc::wrap<float(float, float)>(add)),
        sc::name("sub",     sc::wrap<float(float, float)>(subtract))
        );
        
    // call a function, perhaps with input from your UART? (ignore the return value)
    c.execute("sub 4.2 9.0", 0, nullptr);
```

For more examples, see [example.cc](example.cc).
