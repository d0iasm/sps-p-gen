#!/bin/bash

echo "<h1>$title</h1><ul>" > index.html
for i in *.html; do echo "<li><a href="$i">$i</a></li>"; done >> index.html
echo "</ul>" >> index.html
