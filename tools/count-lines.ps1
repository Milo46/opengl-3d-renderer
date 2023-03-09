Set-Location .\crenderr\source\Crenderr\

Get-ChildItem . -Include ("*.cpp", "*.hpp") -Recurse -Name
    | ForEach-Object { (Get-Content $_).Count }
    | Measure-Object -Sum

Set-Location .\..\..\..\
