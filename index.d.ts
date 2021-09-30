declare module 'fb' {
    export interface Device {
        readonly width: number;
        readonly height: number;

        readonly device: string;

        buffer: Buffer;

        close();
    }

	export interface File {
        readonly width: number;
		readonly height: number;

		readonly fd: number;

		buffer: Buffer;

		close();
    }
	
    export function open_fb(buffer: string): Device;

    export function open_file(file: string, width: number, height: number): File & {file: string};
    export function open_file(fd: number, width: number, height: number): File;
}

export namespace fb {}
