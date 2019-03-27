import java.awt.event.KeyEvent;
import java.io.BufferedReader;
import java.util.Scanner;

import jssc.SerialPortException;

public class PCPlayer {
	
	static String st = "";
	
	public static void main(String[] args) throws SerialPortException, InterruptedException {
		
		SerialComm port = new SerialComm("/dev/cu.usbserial-DN02BJE6");
		
		while (true) {
			
			//if (port.available() && port.readByte() == 0x00) {
				if (StdDraw.isKeyPressed(KeyEvent.VK_LEFT)) {
					port.writeByte((byte)"a".charAt(0));
				}
				else if (StdDraw.isKeyPressed(KeyEvent.VK_UP)) {
					port.writeByte((byte)"w".charAt(0));
				}
				else if (StdDraw.isKeyPressed(KeyEvent.VK_DOWN)) {
					port.writeByte((byte)"s".charAt(0));
				}
				else if (StdDraw.isKeyPressed(KeyEvent.VK_RIGHT)) {
					port.writeByte((byte)"d".charAt(0));
				}
				else {}
				Thread.sleep(20);
			/*if (StdDraw.hasNextKeyTyped()) {
				if (StdDraw.isKeyPressed(KeyEvent.VK_A)) {
					port.writeByte((byte)"a".charAt(0));
				}
				else if (StdDraw.isKeyPressed(KeyEvent.VK_W)) {
					port.writeByte((byte)"w".charAt(0));
				}
				else if (StdDraw.isKeyPressed(KeyEvent.VK_S)) {
					port.writeByte((byte)"s".charAt(0));
				}
				else if (StdDraw.isKeyPressed(KeyEvent.VK_D)) {
					port.writeByte((byte)"d".charAt(0));
				}
				else {}
			}*/
		}
	}

}