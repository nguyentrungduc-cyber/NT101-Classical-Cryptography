# Makefile — Lab01: Mật mã học Cổ điển
# Build tất cả: make
# Build 1 chương trình: make caesar
# Dọn dẹp file binary: make clean

CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall
SRC_DIR = src
TARGETS = caesar substitution playfair vigenere railfence

.PHONY: all clean

all: $(TARGETS)

caesar: $(SRC_DIR)/caesar.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

substitution: $(SRC_DIR)/substitution.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

playfair: $(SRC_DIR)/playfair.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

vigenere: $(SRC_DIR)/vigenere.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

railfence: $(SRC_DIR)/railfence.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)
