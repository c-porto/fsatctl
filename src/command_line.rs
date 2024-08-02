use clap::Parser;

#[derive(Parser, Debug)]
#[command(version, about, long_about)]
struct Args {
    /// Sets verbose logging
    #[arg(short, long)]
    verbose: bool,

    /// Name of the preset to load
    #[arg(short, long)]
    preset: Option<String>,
}

pub fn parse_args() {
    let args = Args::parse(); 

    if args.verbose {
        if let Some(preset) = args.preset {
            println!("Chosen preset is {}",preset);
        }
    }
}
