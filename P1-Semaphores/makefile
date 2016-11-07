## ---------------------------------------------------------------------
## SCD. Makefile para la práctica 1.
## ---------------------------------------------------------------------

.PHONY:  pc, fu, msg, clean, todo
.SUFFIXES:

params_comp:= -g -Wall  
libs:= -lpthread -lrt        

msg:
	@echo "usa 'make pc' para compilar y ejecutar el programa de los productores - consumidores."
	@echo "usa 'make fu' para compilar y ejecutar el programa de los fumadores y el estanquero."
	@echo "usa 'make clean' para borrar todos los archivos creados al compilar."
	@echo "usa 'make todo' para recompilar (si es necesario) los dos programas."

## ---------------------------------------------------------------------

pc: prod-cons	
	./prod-cons

prod-cons: prod-consFIFO.cpp
	g++ -o prod-consFIFO $(params_comp)  prod-consFIFO.cpp $(libs)

## ---------------------------------------------------------------------

fu: fumadores	
	./fumadores

fumadores: fumadores.cpp 
	g++ -o fumadores $(params_comp) fumadores.cpp $(libs)

## ---------------------------------------------------------------------

todo:
	make fumadores
	make prod-cons

## ---------------------------------------------------------------------

clean:
	rm -rf *.o fumadores prod-cons

## ---------------------------------------------------------------------
