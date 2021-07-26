export type fbUtils = {
    readonly width: number,
    readonly height: number,

    readonly device: string,

    data: Buffer;
};

export function open(device: string): fbUtils;
