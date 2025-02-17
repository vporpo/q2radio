all:
	$(CXX) -O q2radio.cpp -lusb-1.0 -o q2radio

windows:
	$(CXX) -O q2radio.cpp -lusb-1.0 -o q2radio -DWINDOWS

# For development use
win_dst=q2radio_win
windows_release: windows
	mkdir -p $(win_dst)
	zip -r $(win_dst).zip $(win_dst)/

install:
	cp q2radio /usr/local/bin/
clean:
	rm q2radio
