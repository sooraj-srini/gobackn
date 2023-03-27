all:
	g++ -o SenderGBN SenderGBN.cpp
	g++ -o ReceiverGBN ReceiverGBN.cpp
clean:
	rm SenderGBN ReceiverGBN