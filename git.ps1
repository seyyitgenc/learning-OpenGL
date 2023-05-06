$committag = $args[0]

Write-Host "Executing git commands ..."

git add .
git commit -m " $committag "
git push origin main