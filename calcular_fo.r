b <- as.matrix(read.csv("b.csv", header = TRUE))
B_inversa <- as.matrix(read.csv("B_inversa.csv", header = TRUE))
Cb <- as.matrix(read.csv("Cb.csv", header = FALSE))

print(Cb)
print(B_inversa)
print(b)

fo <- Cb%*%B_inversa%*%b

write.csv(fo, file = "fo.csv", row.names = FALSE)