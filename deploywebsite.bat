@echo off
path=%path%;c:\cygwin\bin
rsync -avz --no-p website/* mark@thelinux:/var/www/html/planetgraphs --exclude=data/*