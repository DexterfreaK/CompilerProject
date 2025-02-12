# lexerog: src/lexerog.c
# 	gcc src/lexerog.c && ./a.out test/lexer_tc/t2.txt
lexer: src/lexer_nstb.c
	gcc src/alter_lexer/lexerog.c && ./a.out test/parser_tc/t6.txt
