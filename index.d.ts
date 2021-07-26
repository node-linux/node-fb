declare module 'nodefb' {
    export type fbUtils = {
        readonly width: number,
        readonly height: number,

        readonly device: string,

        buffer: Buffer;

        // flush(buffer: Buffer);
    };

    export function open(device: string): fbUtils;
}
