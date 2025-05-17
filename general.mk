include config.mk

all: $(TARGET).a libSimpleOfuscator.a
	ar -t $^
	gcc $(CFLAGS) $(INCLUDE_FLAGS) $(PATH_EXAMPLES)/code.c $(LINKER_FLAGS) -o code.$(EXTENSION)

$(TARGET).a: $(OBJECTS)
	$(ARR) $(ARR_FLAGS) $@ $^
	ranlib $@

libSimpleOfuscator.a:
	$(MAKE) -C ./$(PATH_SimpleOfuscator) -f $(MAKE_NAME)

GetCommandOutputC.o: $(PATH_SRC)/GetCommandOutputC.c
	$(CC) $(CFLAGS) -c $^ -o $@

cleanobj:
	$(RM) $(RMFLAGS) $(OBJECTS)
	$(MAKE) -C ./$(PATH_SimpleOfuscator) -f $(MAKE_NAME) cleanobj

cleanall: cleanobj
	$(RM) $(RMFLAGS) *.o $(TARGET).a \
	$(TARGET_DEBUG).a *.$(EXTENSION) 
	$(MAKE) -C ./$(PATH_SimpleOfuscator) -f $(MAKE_NAME) cleanall

.SILENT: clean cleanobj cleanall
.IGNORE: cleanobj cleanall
.PHONY:  cleanobj cleanall