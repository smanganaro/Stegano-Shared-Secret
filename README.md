ESTEGANOGRAFIA - TP CRIPTOGRAFIA Y SEGURIDAD

Modulos:

1) Parser de comandos por linea
2) Manejo de archivos bmp
3) Operaciones matriciales
4) Esquema propuesto a nivel matriz 
5) Esteganografiado

Flujo aproximado:

Se ingresan los comandos por linea con los parametros. Puede ser una encripcion
o una desencripcion. Se pasan dos archivos de imagen, uno es la imagen a
encriptar/crear y el segundo es el watermark, los numeros para esquema y un 
directorio, que tenemos que abrir y chequear que tenga los shares, o donde 
poner los shares.

Encripcion:

- Recibimos la imagen en formato bmp en blanco y negro (8bits x pixel) y 
una imagen bmp que servira como watermark para verificar los shares.
Ademas nos pasan un esquema (k,n)

- Construimos una matriz con valores aleatorios mxk donde m > 2(k-1)-1
	Nota: como solo tenemos esquema 2,4 y 4,8 las matrices A van a ser de 
	2(o mas) x2 o de 6(o mas) x4.
- Calculamos la matriz dobleS = proj(A) mod 251
	Para esto tenemos que poder calcular matrices inversas y transpuestas
	ademas de multiplicar matrices
- Calculamos la matriz R = (dobleS - matriz secreta) mod 251
	Para esto vamos a tener que poder hacer resta de matrices con modulo
- Creamos n vectores de k filas Xj con valores aleatorios y n escalares Cj 
	Todo con 1 <= j <= n
- Calculamos los vectores vj=(A x Xj) mod 251
	Para esto necesitamos hacer producto matricial con modulo 251
- Creamos las matrices Gj (para los valores previos de j) las cuales tienen m/k
	columnas y filas igual a los vectores vj.
	Para hacer esto tenemos Gj = [g1;1 g2;1 g3;1 ... gm/k;1] (una sola fila)
	Donde g1;1 = I(1k + 1, j) + ... + I(1k + k - 1, j)r1;k-1 mod 251
	Donde I(i,j) es el pixel en la fila i columna j de la imagen secreta
	Para esto vamos a tener que poder manipular pixeles de imagenes como
	matrices.
- Seleccionamos la imagen watermark W y calculamos Rw= (W-dobleS) mod 251

- Dividir las shares Shj = [vjGj] a los participantes y hacer Rw publica
	Para esto necesitamos producto de matrices
Desencripcion

- Seleccionamos k shares de los participantes
	Para esto vamos a tener que poder levangtar y leer archivos
- Construir la matriz B usando la primer columna de cada share
	B = [ Sh1(;,1) .... Shk(:,1) ]
- Construir la matriz dobleS donde dobleS=(proj(B)) mod251
- Construir la matriz Gj con las columnas de los shares no utilizados para la 
	matriz B
	G1 = [ Sh1(:,2) Sh1(:,3) ... Sh1(:,k) ]
- Usar las matrices Gj para reconstruir R
	Esta ecuacion es un bardo y no se de donde mierda salen los cj.
	Esta en la pag 6/7 del apunte de Azzhara
- Una vez calculada R se construye la matriz secreta S = (dobleS + R) mod 251
	Y se verifica usando la W, calculando W= dobleS+Rw mod 251

Manejo de archivos BMP:

Chequear que la imagen sea divisible en matrices de nxn disjuntas. La imagen
secreta debe ser de 8 bits por pixel. Chequear que la imagen no tenga metadata
al final.

Esteganografia

Los shares son de 24 bits por pixel
Esquema 4,8

Si los shares no tienen el mismo tamano que la imagen secreta, se muestra un
mensaje de error y no se realiza nada.
Se toman los valores a ocultar, y para cada byte de la imagen se cambia el
ultimo bit por el siguiente bit del byte a ocultar.
El numero del share se ocultara en el byte 6 del header archivo bmp

Esquema 2,4
Si los shares no tienen el mismo tamano que la imagen secreta, se muestra un
mensaje de error y no se realiza nada.
Se hace lo mismo que el anterior pero en vez de ocultar de a un bit se oculta
de a dos bit.

Marca de agua
Los bytes de la marca de agua se guardan en una imagen byn (8bits x pixel).

Imagenes portadoras
Las imagenes portadoras deben ser de 24 bits x pixel y la resolucion es de 
8 bits x pixel.





