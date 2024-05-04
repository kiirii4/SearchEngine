INCLUDES:=../include
SRCS:=$(wildcard *.cpp) 
LIBS:= -lpthread
OBJS:=$(patsubst %.cpp, %.o, $(SRCS))
SERVER:=main

# 目标 : 依赖
$(SERVER): $(OBJS)
	@#g++ $^ -o $@ $(LIBS) $(addprefix -I, $(INCLUDES)) -g
	g++ $^ -o $@ $(LIBS) -g

%.o:%.cpp
	@#g++ -c $^ -o $@ $(addprefix -I, $(INCLUDES)) -g
	g++ -c $^ -o $@ -g

.PHONY:echo clean
echo:
	echo $(INCLUDES)
	echo $(SRCS)

clean:
	rm -rf $(OBJS) $(SERVER)
