#!/usr/bin/env bash

# Enviamos los strings limpios directamente al puerto TCP
echo -n "SET clave_prueba" | nc localhost 8080
echo -n "GET clave_prueba" | nc localhost 8080
echo -n "GET clave_fantasma" | nc localhost 8080
echo -n "DELETE clave_prueba" | nc localhost 8080

