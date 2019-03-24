//import communication.SerialComm;
import jssc.*;

public class SerialComm {

	static SerialPort port;

	private boolean debug;  // Indicator of "debugging mode"
	
	// This function can be called to enable or disable "debugging mode"
	void setDebug(boolean mode) {
		debug = mode;
	}	
	

	// Constructor for the SerialComm class
	public SerialComm(String name) throws SerialPortException {
		port = new SerialPort(name);		
		port.openPort();
		port.setParams(SerialPort.BAUDRATE_9600,
			SerialPort.DATABITS_8,
			SerialPort.STOPBITS_1,
			SerialPort.PARITY_NONE);
		
		debug = false; // Default is to NOT be in debug mode
	}
		
	// TODO: Add writeByte() method 
	public void writeByte(byte singleByte) {
		try {
			port.writeByte(singleByte);
			setDebug(true);
		} catch (SerialPortException e) {
			e.printStackTrace();
			setDebug(false);
		}
		if (debug == true) {
			String x = String.format("%02X", singleByte & 0xff);
			System.out.println(x);
		}
	}
	
	// TODO: Add available() method
	boolean available() {
		try {
			if (port.getInputBufferBytesCount() > 0) {
				return true;
			}
		} catch (SerialPortException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return false;
	}	
	
	// TODO: Add readByte() method	
	byte readByte() throws SerialPortException {
		if (debug == true) {
			 
			String x = String.format("%02X", port.readBytes(port.getInputBufferBytesCount())[0] & 0xff);
			System.out.print(x);
			return port.readBytes(1)[0]; 
		}
		return port.readBytes(1)[0];
		//port.getInputBufferBytesCount()
	}	
	
	// TODO: Add a main() method
	public static void main(String[] args) {
		SerialComm objects;
		try {
			objects = new SerialComm("/dev/cu.usbserial-DN03FDMZ");
			while (true) {
				if (objects.available()) {
					if (objects.debug) {
						byte x = port.readBytes(1)[0];
						String xx = String.format("%02X", x & 0xff);
						System.out.println(xx);
					}
				
				}
			}
		} catch (SerialPortException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} 
	}

}
