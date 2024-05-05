SHARED:=../shared
LIBS:= -lpthread -llog4cpp
SRCS:=$(wildcard *.cpp) 
OBJS:=$(patsubst %.cpp, %.o, $(SRCS))
SHARED_SRCS:=$(wildcard $(SHARED)/*.cpp)
SHARED_OBJS:=$(patsubst $(SHARED)/%.cpp, $(SHARED)/%.o, $(SHARED_SRCS))
OUT:=../../bin/webOffline

# 目标 : 依赖
$(OUT): $(OBJS) $(SHARED_OBJS)
	g++ $^ -o $@ $(LIBS) -g

$(OBJS): %.o : %.cpp
	g++ -c $^ -o $@ -g

$(SHARED_OBJS): $(SHARED)/%.o : $(SHARED)/%.cpp
	g++ -c $^ -o $@ -g
.PHONY: clean rebuild

clean:
	rm -rf $(OBJS) $(OUT) $(SHARED_OBJS)

rebuild: clean $(OUT)
