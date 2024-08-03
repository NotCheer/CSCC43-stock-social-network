CC=g++

CFLAGS=-std=c++11 -Isrc/dao -Isrc/obj

LIBS=-lpqxx -lpq

SRC=src/main.cpp \
    src/dao/FriendshipDao.cpp \
    src/dao/PortfolioDao.cpp \
    src/dao/PortfolioHoldingDao.cpp \
    src/dao/PortfolioStatDao.cpp \
    src/dao/ReviewDao.cpp \
    src/dao/StockDao.cpp \
    src/dao/StockListDao.cpp \
    src/dao/StockListHoldingDao.cpp \
    src/dao/UserDao.cpp \
    src/obj/Friendship.cpp \
    src/obj/Portfolio.cpp \
    src/obj/PortfolioHolding.cpp \
    src/obj/PortfolioStat.cpp \
    src/obj/Review.cpp \
    src/obj/Stock.cpp \
    src/obj/StockList.cpp \
    src/obj/StockListHolding.cpp \
    src/obj/User.cpp

OBJ=$(addprefix build/, $(notdir $(SRC:.cpp=.o)))

vpath %.cpp $(sort $(dir $(SRC)))

TARGET=main

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LIBS)

build/%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f build/*.o $(TARGET)
