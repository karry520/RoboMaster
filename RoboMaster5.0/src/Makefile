.PHONY:all

all:$(OBJ_DIR)/ToolKit.o $(OBJ_DIR)/Identify.o $(OBJ_DIR)/Auto_Attack_Mode.o $(OBJ_DIR)/Rune_Mode.o $(OBJ_DIR)/Standby_Mode.o

$(OBJ_DIR)/ToolKit.o:ToolKit.cpp
	$(CC) -c $^ -o $@ $(CFLAG)

$(OBJ_DIR)/Identify.o:Identify.cpp
	$(CC) -c $^ -o $@ $(CFLAG)

$(OBJ_DIR)/Auto_Attack_Mode.o:Auto_Attack_Mode.cpp
	$(CC) -c $^ -o $@ $(CFLAG)

$(OBJ_DIR)/Rune_Mode.o:Rune_Mode.cpp
	$(CC) -c $^ -o $@ $(CFLAG)

$(OBJ_DIR)/Standby_Mode.o:Standby_Mode.cpp
	$(CC) -c $^ -o $@ $(CFLAG)
