#include <linux/module.h>
#include <linux/pci.h>

#define NVIDIA_VENDOR_ID 0x10de
#define GF119_DEVICE_ID  0x104a

static int __init gt610_init(void)
{
    struct pci_dev *pdev = NULL;

    // Print starting to the kernel
    // log.
    pr_info("GT610-EXP: starting\n");

    // Scan the system's PCI bus for a device matching
    // vendor `0x10de` and device `0x104a`.
    // The third argument: `NULL` means `start searching
    // from the beginning`. (It's a cursor for iteration).

    // It returns a refernce-counted pointer to `struct pci_dev`,
    // if found, or `NULL` if not found.
    pdev = pci_get_device(NVIDIA_VENDOR_ID, GF119_DEVICE_ID, NULL);

    // If the device is not found, it logs the error
    // message together with 0 (success), we do not
    // return an error because we did load succesfully.
    if (!pdev) {
        pr_info("GT610-EXP: GF119 not found\n");
        return 0;
    }

    // If found, pci_name(pdev) returns a string like
    // "0000:01:00.0" (9DF — Bus:Device.Function address).
    // Logs the vendor and device IDs as hex values for
    // confirmation.
    
    pr_info("GT610-EXP: FOUND %s\n", pci_name(pdev));
    pr_info("GT610-EXP: vendor=0x%04x device=0x%04x\n",
            pdev->vendor, pdev->device);

    // Logs BAR0 (*Base Address Register 0) information:
    // - pdev->resource[0] is BAR0 — for NVIDIA GPUs.
    // -  &pdev->resource[0].start and &pdev->resource[0].end
    // are the physical start/end addresses of this BAR region.
    // - The %pa format specifier is the kernel's way to print
    // resource_size_t values (physical addresses)
    resource_size_t bar0_size = resource_size(&pdev->resource[0]);
    pr_info("GT610-EXP: BAR0 start=%pa size=%pa\n",
            &pdev->resource[0].start,
            &bar0_size);

    //  decrements the reference count on the PCI device struct,
    // allowing it to be freed when no longer in use.
    // This is required to avoid a reference leak.
    pci_dev_put(pdev);

    // Return 0, success.
    return 0;
}

// This loads when we run `rmmod`
static void __exit gt610_exit(void)
{
    pr_info("GT610-EXP: stopped\n");
}

// We register the init and exit functions,
// with the kernel module framework.
module_init(gt610_init);
module_exit(gt610_exit);

// CRITICAL: MARK AS GPL.
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kernel Experiment");
MODULE_DESCRIPTION("Experimental GF119 GT 610 PCI inspector");
