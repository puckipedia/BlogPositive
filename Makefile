run: clean blogpositive
	./blogpositive
clean:
	rm blogpositive || true
blogpositive:
	gcc -g -std=c90 -lroot -lbe -Wno-deprecated -o blogpositive 
**.cpp */*.cpp
