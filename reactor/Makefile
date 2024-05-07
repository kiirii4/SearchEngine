SHARED:=../shared
WEB:=../web_online
KEY:=../keyword_online
LIBS:= -lpthread -llog4cpp 
SRCS:=$(wildcard *.cpp) 
OBJS:=$(patsubst %.cpp, %.o, $(SRCS))
SHARED_SRCS:=$(wildcard $(SHARED)/*.cpp)
SHARED_OBJS:=$(patsubst $(SHARED)/%.cpp, $(SHARED)/%.o, $(SHARED_SRCS))
KEY_SRCS:=$(wildcard $(KEY)/*.cpp)
KEY_OBJS:=$(patsubst $(KEY)/%.cpp, $(KEY)/%.o, $(KEY_SRCS))
WEB_SRCS:=$(wildcard $(WEB)/*.cpp)
WEB_OBJS:=$(patsubst $(WEB)/%.cpp, $(WEB)/%.o, $(WEB_SRCS))
OUT:=../../bin/server

# 目标 : 依赖
$(OUT): $(OBJS) $(SHARED_OBJS) $(KEY_OBJS) $(WEB_OBJS)
	g++ $^ -o $@ $(LIBS) -g

$(OBJS): %.o : %.cpp
	g++ -c $^ -o $@ -g

$(SHARED_OBJS): $(SHARED)/%.o : $(SHARED)/%.cpp
	g++ -c $^ -o $@ -g

$(WEB_OBJS): $(WEB)/%.o : $(WEB)/%.cpp
	g++ -c $^ -o $@ -g

$(KEY_OBJS): $(KEY)/%.o : $(KEY)/%.cpp
	g++ -c $^ -o $@ -g

.PHONY: clean rebuild

clean:
	rm -rf $(OBJS) $(OUT) $(SHARED_OBJS) $(KEY_OBJS) $(WEB_OBJS)

rebuild: clean $(OUT)
