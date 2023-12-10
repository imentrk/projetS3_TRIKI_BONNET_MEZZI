cmake_minimum_required(VERSION 3.24)
project(projetSM3algo C)

set(CMAKE_C_STANDARD 11)

add_executable(projetSM3algo main.c cell/cell.c cell/cell.h list/list.h list/list.c agenda/agenda.h agenda/agenda.c timer/timer.c timer/timer.h)
