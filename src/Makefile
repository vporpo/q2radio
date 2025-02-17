all:
	$(CXX) -O q2radio.cpp -lusb-1.0 -o q2radio

windows:
	$(CXX) -O q2radio.cpp -lusb-1.0 -o q2radio -DWINDOWS

install:
	cp q2radio /usr/local/bin/
clean:
	rm q2radio
