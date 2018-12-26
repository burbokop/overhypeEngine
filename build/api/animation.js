'use strict';

const api = {
    oh: require('overhype'),
};


class Animation {
	
	/*
    static PING_PONG = 0;
	static LOOP = 1;
	static ONEFRAME = 2;
	static FROZE = 3;
	static NOTRENDER = 4;
	static TOTHEFRAME = 5;
	*/
	
    constructor(source, w, h, mode, interval) {
        this.mode = mode;
		this.timer = new api.oh.math.Timer(interval ? interval : 500);


		this.frames = [];
		this.frames.push(api.oh.spm.slice(source, 0, 0, w, h));
		this.frames.push(source);
    }

	/*
	play(mode, mirror, track, number) {
		this.mode = mode;
		this.mirror = mirror;
		this.track = track;
		this.number = number;
	}
	*/
    
    /*
	public boolean isCompleted() {
		if(this.mode == AnimationFrame.TOTHEFRAME) {
			if(this.iterator == this.number){
				return true;
			}
			return false;
		}
		return true;
    }
    */
	/*
	slice(x, y, scale) {
		if(this.timer.count() && this.mode != AnimationFrame.FROZE){
			if(this.number == 0) {
				this.iterator = 0;
			}
			else {	
				if(this.mode == AnimationFrame.PING_PONG) {
					this.iterator += this.iteratorDerivative;
					if(this.iterator >= this.number - 1) this.iteratorDerivative = -1;
					if(this.iterator <= 0) {
						this.iteratorDerivative = 1;
						this.iterator = 0;
					}
				}
				else if (this.mode == AnimationFrame.LOOP) {
					this.iterator++;
					if(this.iterator >= this.number) this.iterator = 0;
				}
				else if (this.mode == AnimationFrame.TOTHEFRAME) {
					if(this.iterator > this.number) this.iterator--;
					if(this.iterator < this.number) this.iterator++;
					else this.iterator = this.number;
				}
				else if (this.mode == AnimationFrame.ONEFRAME) {
					this.iterator = this.number;
				}
			}
			this.track2 = this.track;
		}
	
		BufferedImage frame = null;
		try {
			frame = this.source.getSubimage(frameW * iterator, frameH * this.track2, frameW, frameH);
		}
		catch (Exception e) {
			System.err.println("frame error (iterator: " + iterator + "; track: " + this.track2 + ")");
		}
	
		if(frame != null) {
			AffineTransform at = new AffineTransform();
			if(this.mirror == null) this.mirror = new Mirror(Mirror.N);
			
			System.out.println(frame.getWidth() + " " + this.mirror.getIntHorizontal());
			
			if(centered) at.translate(x + (this.offsetX - frame.getWidth()) * this.mirror.getIntHorizontal(), y + (this.offsetY - frame.getHeight()) * this.mirror.getIntVertical());
			else at.translate(x, y);
			at.scale(scale * this.mirror.getIntHorizontal(), scale * this.mirror.getIntVertical());
			
			return new TransformedFrame(frame, at);
		}
		return null;
	}
	
	public void render(Graphics graphics, int x, int y, float scale, boolean centered) {
		if(this.mode == AnimationFrame.NOTRENDER) return;
		TransformedFrame transformedFrame = this.slice(x, y, scale, centered);
		if(transformedFrame != null) {
			if(this.applyOffsetToRotateCentre) transformedFrame.transform.rotate(this.angle * this.mirror.getMultiplier(), this.frameW / 2 + this.offsetX, this.frameH / 2 + this.offsetY);
			else transformedFrame.transform.rotate(this.angle * this.mirror.getMultiplier(), this.frameW / 2, this.frameH / 2);
			transformedFrame.draw(graphics);
		}
	}
    

	*/

    render(surface, offset, offsetLocal) {
		
        if (surface !== undefined) {
			console.log(this.frames);

			const pos = {}
            const x = offset.coords[0];
            const y = offset.coords[1];
            
            //const localPos = this.parent.pos.subtract(offset).add(offsetLocal);
            //const x = localPos.x;
            //const y = localPos.y;
            api.oh.spm.rotatedSurface(
                this.frames[0],
                surface,
                { x, y },
                this.angle ? (this.angle * 180 / -Math.PI) : 0,
                this.zoom !== undefined ? this.zoom : 1
            );
        }
    }
}


module.exports = Animation;