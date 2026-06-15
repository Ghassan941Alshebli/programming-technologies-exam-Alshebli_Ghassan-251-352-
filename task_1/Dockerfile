# 1. В качестве исходного образа взять fedora:36
FROM fedora:36

# 2. Добавить комментарий 
# QT TCP Server (Fedora-based)

# 3. Установить Qt5 и компилятор
# تحديث النظام وتثبيت حزم Qt5 ومترجم g++ (في فيدورا يسمى gcc-c++)
RUN dnf -y update && \
    dnf -y install qt5-qtbase-devel gcc-c++ make

# 4. Скопировать файлы проекта в контейнер в папку /var/lib/server
# تحديد مسار العمل ونسخ الملفات إليه
WORKDIR /var/lib/server
COPY . /var/lib/server/

# 5. Скомпилировать проект в файл с именем fedora_server
# سنقوم بتحديث هذا السطر بناءً على ملفات المهمة 2، لكن مبدئياً سيكون هكذا:
RUN qmake-qt5 && make && mv server fedora_server 

# 6. Открыть порт 65432 и запускать сервер при старте
EXPOSE 65432
CMD ["./fedora_server"]