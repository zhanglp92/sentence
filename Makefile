# Makefile 0.1


SENTENCE 	:= sentence
CLEAN_DIGUI := clean_recursion_L
FIRST_FL 	:= first_follow
ANA_TABLE 	:= analyse_table
TOOLS 		:= tools
IS_LL1 		:= decide_LL1
ANA_STACK 	:= analyse_stack
BIN 		:= a.out
OBJ_FILE 	:= $(SENTENCE).o $(CLEAN_DIGUI).o $(FIRST_FL).o $(TOOLS).o $(ANA_TABLE).o $(IS_LL1).o $(ANA_STACK).o

IDIR 		:= ./include
SDIR 		:= ./src
LINK_LIB 	:= -I$(IDIR)

# =============================================
# 目标文件
# =============================================
$(BIN) : $(OBJ_FILE)
	cc -o $(BIN) $(OBJ_FILE) $(LINK_LIB) -g
	make clean
# =============================================

# =============================================
# 编译过程
# =============================================

$(OBJ_FILE) : $(SDIR) $(IDIR)
	cc -c $(SDIR)/$(CLEAN_DIGUI).c $(LINK_LIB) -g
	cc -c $(SDIR)/$(SENTENCE).c $(LINK_LIB) -g
	cc -c $(SDIR)/$(FIRST_FL).c $(LINK_LIB) -g
	cc -c $(SDIR)/$(ANA_TABLE).c $(LINK_LIB) -g
	cc -c $(SDIR)/$(TOOLS).c $(LINK_LIB) -g
	cc -c $(SDIR)/$(IS_LL1).c $(LINK_LIB) -g
	cc -c $(SDIR)/$(ANA_STACK).c $(LINK_LIB) -g
# ============================================= 

.PHONY : clean

clean :
	-rm *.o
