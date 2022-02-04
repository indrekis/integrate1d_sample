# Lab work 2: sample for numerical integration -- 1d
Authors: Oleg Farenyuk: [https://github.com/indrekis]() <br>
Variant: None
## Prerequisites

No additional prerequirities

### Compilation

    mkdir <build-dir>
    cd <build-dir>
    cmake <path-to-src>
    cmake --build .
See also compile.sh

### Installation

    cmake --install . # Optional

### Usage
    intergate1d_sample <config-file>
or
    intergate1d_sample
Without an argument uses "integration_1d.cfg" in the current directory as a default configuration. 

### Important!

Реалізація однопоточна і лише для функції однієї змінної. Код цей -- для ілюстрації, 
ви не повинні копіювати його бездумно. Зокрема, читання конфігурування використовує
засоби С++, які помітно складніші, ніж ті, що ми систематично розглядали -- 
я вимагатиму їх пояснити, якщо скопіюєте бездумно і без розуміння (-1 і більше балів).

Також, залишені мої коментарі для вас виглядатимуть дивно у зданому вами коді. 
(-0.25, за кожен у екстремальних випадках).

PVS Studio, санітайзери та errors as warning по замовчуванню вимкнені -- однак, 
обов'язково вмикайте під час розробки -- це дуже спростить вам життя! (Навіть
якщо на початках буде здаватися по іншому). 

### Results

PSV Studio, valgrind і більшість санітайзерів не скаржаться. Виняток -- MSAN у мене
скаржиться на реалізацію потоків вводу-виводу у глибині стандартної бібліотеки 
(std::__basic_file::open -- use of uninitialized value). Навіть якщо це не false positive,
турбуватися про це попередження виходить за межі того, що вимагається від студентів. :-)

# Additional tasks
None 

