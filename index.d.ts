declare module 'fb' {
	export interface Device {
		readonly width: number;
		readonly height: number;

		readonly device: string;

		buffer: Buffer;

		close();
	}
	
    export function open(device: string): Device;
}
