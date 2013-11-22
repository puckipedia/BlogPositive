run: clean blogpositive
	./blogpositive
clean:
	rm blogpositive || true
blogpositive:
	gcc -g -lroot -lbe -Wno-deprecated -o blogpositive **.cpp */*.cpp
