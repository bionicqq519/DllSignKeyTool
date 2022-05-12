# DllSignKeyTool
## Server side
Ipconfig 搜尋本機IP提供給Client端
![image](https://user-images.githubusercontent.com/10304041/168124772-7534e1ba-1e2a-49bc-b6da-32a8813803b6.png)

找一個working path執行檔案
![image](https://user-images.githubusercontent.com/10304041/168124793-ec9df294-bba7-4624-933b-44dde2bf47bf.png)

輸入: 1
![image](https://user-images.githubusercontent.com/10304041/168124804-891c84cd-ff95-4ede-b1b5-255f7e80be8e.png)

成功會顯示以下訊息
![image](https://user-images.githubusercontent.com/10304041/168124826-d978c042-d244-4663-9724-1f6cd1218510.png)

## Client side – 輸入IP
1. working path放入exe, 欲sign key的library(檔名”alPP_x64.dll”寫死)
![image](https://user-images.githubusercontent.com/10304041/168125067-7b1f193e-899e-46e4-9731-ca9002b3a6e5.png)
2. 輸入: 2
3. 輸入Server IP
4. Sign 完成，檔案在同路徑
![image](https://user-images.githubusercontent.com/10304041/168125099-5ccb0d33-381b-4a4a-acbc-d5aafc539d72.png)

## Client side – 使用bat代入IP
1. 編輯sign.bat代入Server IP
![image](https://user-images.githubusercontent.com/10304041/168125198-15c5fc69-f443-4786-8f95-753f41db259d.png)
2. 執行bat
3. 輸入: 2
4. Sign key完成
![image](https://user-images.githubusercontent.com/10304041/168125261-f00d4130-372c-4019-81ea-3cf1eb11ca72.png)

## Sign key 完成
完成後會建立一個alPP_x64_sign.dll
![image](https://user-images.githubusercontent.com/10304041/168125356-9785a057-971c-4b88-9e3e-369d3ddcd1a5.png)





