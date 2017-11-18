rm UDPServer.jar
rm UDPClient.jar
javac xyz/rinc/udp/UDPServer.java xyz/rinc/udp/UDPClient.java
jar cvf UDPServer.jar xyz/rinc/udp/UDPServer.class
jar cvf UDPClient.jar xyz/rinc/udp/UDPClient.class 'xyz/rinc/udp/UDPClient$Callback.class'
rm xyz/rinc/udp/*.class