FROM gcc:latest


WORKDIR /app

COPY source.cpp .

RUN g++ -o run.out source.cpp -lm -lncursesw

CMD ["./run.out"]
