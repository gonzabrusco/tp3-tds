# Trabajo Práctico Número 3

Testing de un modulo de software para comunicacion PowerSPI (un mix entre SPI e I2C)

## Uso del repositorio

Este repositorio utiliza [pre-commit](https://pre-commit.com) para validaciones de formato. Para trabajar con el mismo usted debería tener instalado:

1. pre-commit (https://pre-commit.com/#install)
2. cpp-check (https://cppcheck.sourceforge.io)

Después de clonar el repositorio usted debería ejecutar el siguiente comando:

```
pre-commit install
```

Para generar la documentación del proyecto se utiliza el siguiente comando:

```
make doc
```

Para correr los tests, se debe ejecutar ceedling dentro de la carpeta del modulo:

```
cd Drivers/powerspi/
ceedling
```
