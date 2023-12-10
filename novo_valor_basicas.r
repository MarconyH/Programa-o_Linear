b <- as.matrix(read.csv("b.csv", header = FALSE))
B_inversa <- as.matrix(read.csv("B_inversa.csv", header = TRUE))

b <- t(b)

b <- B_inversa%*%b

write.csv(b, file = "b.csv", row.names = FALSE)

