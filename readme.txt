为了在一台电脑上调试ip用了127.0.0.1，端口1900（客户端），63159（monitor）

client和monitor运行后enter连接server
server需要在连接前先运行

程序中虽然有check（）函数，但是没有实现，也就是说client输入时
如果输入不符合要求的内容（如35=d，44=abc，38=x等）可能有错
（cancellation acknowledge和cancellation reject可以实现）

client输入按fix message的格式，比较简陋。