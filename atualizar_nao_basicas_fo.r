# Lê o vetor Cb do arquivo CSV
Cb <- as.matrix(read.csv("Cb.csv", header = FALSE))

# Lê o vetor Cr do arquivo CSV
Cr <- as.matrix(read.csv("Cr.csv", header = FALSE))

# Lê a matriz B do arquivo CSV
B <- as.matrix(read.csv("B.csv", header = FALSE))

# Lê a matriz R do arquivo CSV
R <- as.matrix(read.csv("R.csv", header = FALSE))

# print(B)
# print(Cb)
# print(R)
# print(Cr)

B_inversa <- solve(B)

print(dim(Cb))
print(dim(B_inversa))
print(dim(R))

Cr = Cr - Cb %*% B_inversa %*% R

# print(B)
# print(B_inversa)
# print(Cb)
# print(R)
# print(Cr)


# Escrever a matriz B_inversa em um arquivo CSV
write.csv(B_inversa, file = "B_inversa.csv", row.names = FALSE)

# Escrever o vetor Cr em um arquivo CSV
write.csv(Cr, file = "Cr.csv", row.names = FALSE)