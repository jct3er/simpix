import sys
import numpy as np
from PIL import Image
import matplotlib.pyplot as plt

def main():
    if len(sys.argv) < 3:
        print("Usage: simapix_start image1 image2 <output=out.png>")
        return

    fsrc = sys.argv[1]
    ftgt = sys.argv[2]
    fout = sys.argv[3] if len(sys.argv) > 3 else "out.png"
    
    print(f"Reading images: source= {fsrc} target= {ftgt}")
    print(f"Output= {fout}")

    # Open images and ensure they are the same size
    src = Image.open(fsrc)
    tgt = Image.open(ftgt)

    if src.size != tgt.size:
        raise ValueError("Images do not have the same dimensions.")

    print(f"Pixel Geometry: {src.width} x {src.height}")
    numPix = src.width * src.height

    # Convert images to numpy arrays
    src_array = np.array(src)
    tgt_array = np.array(tgt)
    out_array = np.copy(src_array)  # Start with a copy of the source

    # Manipulate pixels - turn off green
    out_array[:, :, 1] = 0  # Set green channel to 0

    # Flip the image
    # out_array = np.flipud(out_array)

    # Create a collage of images
    fig, axs = plt.subplots(2, 2, figsize=(8, 4))
    
    axs[0,0].imshow(src)
    axs[0,0].set_title('Source Image')
    axs[0,0].axis('off')
    
    axs[0,1].imshow(tgt)
    axs[0,1].set_title('Target Image')
    axs[0,1].axis('off')
    
    axs[1, 0].imshow(out_array)
    axs[1, 0].set_title('Output Image')
    axs[1, 0].axis('off')

    axs[1, 1].axis('off')
    
    plt.tight_layout()
    plt.savefig("collage.png")
    
    # Save the new image
    out_image = Image.fromarray(out_array)
    out_image.save(fout)

    print("Clsoe image window to exit")
    plt.show()

if __name__ == "__main__":
    main()

    
