
EXAMPLE_SRC = $(wildcard examples/*.cpp)
EXAMPLE_OUT = $(EXAMPLE_SRC:.cpp=.out)

CPPFLAGS = -Wall -Wextra -std=c++11

all: $(EXAMPLE_OUT)

$(EXAMPLE_OUT): %.out: %.cpp
	g++ $(CPPFLAGS) $< -o $@ -Iinclude

clean:
	rm -f $(EXAMPLE_OUT)
