const fs = require('fs/promises');
const path = require('path');

// Simple script to minify `.agent` markdown files to reduce token bloat for CLI workers
// Use when packaging or before large swarm workflows.
// UPGRADED: Uses async/await and Promise.all for parallel I/O performance.

const args = process.argv.slice(2);
const shouldWrite = args.includes('--write');

if (!shouldWrite) {
  console.log('DRY RUN MODE: Pass --write to actually modify files.');
}

const AGENT_DIR = path.resolve(__dirname, '..');

function minifyMarkdown(content) {
  // 1. Remove excessive empty lines (more than 2 consecutive newlines -> 2)
  let minified = content.replace(/\n{3,}/g, '\n\n');
  
  // 2. Remove HTML comments <!-- ... --> EXCEPT <!-- slide --> (for carousels)
  minified = minified.replace(/<!--(?!\s*slide\s*-->)[\s\S]*?-->/g, '');
  
  // 3. Trim trailing whitespace but preserve markdown hard breaks (two spaces)
  minified = minified.split('\n').map(line => {
    const hasHardBreak = line.endsWith('  ');
    return line.trimEnd() + (hasHardBreak ? '  ' : '');
  }).join('\n');
  
  return minified.trim() + '\n';
}

async function traverseAndMinify(dir) {
  const files = await fs.readdir(dir);
  const tasks = files.map(async (file) => {
    if (['spawn_agent_tasks', 'benchmarks', '.hc', '.git', 'node_modules'].includes(file)) return;
    
    const fullPath = path.join(dir, file);
    try {
      const stats = await fs.stat(fullPath);
      
      if (stats.isDirectory()) {
        await traverseAndMinify(fullPath);
      } else if (file.endsWith('.md')) {
        const original = await fs.readFile(fullPath, 'utf8');
        const minified = minifyMarkdown(original);
        
        if (original !== minified) {
          if (shouldWrite) {
            await fs.writeFile(fullPath, minified, 'utf8');
            console.log(`Minified: ${path.relative(AGENT_DIR, fullPath)}`);
          } else {
            console.log(`[DRY RUN] Would minify: ${path.relative(AGENT_DIR, fullPath)}`);
          }
        }
      }
    } catch (err) {
      console.error(`Error processing ${fullPath}:`, err.message);
    }
  });

  await Promise.all(tasks);
}

async function main() {
  console.time('Minification Time');
  console.log('Starting parallel minification of /.agent framework...');
  try {
    await traverseAndMinify(AGENT_DIR);
    console.log('Done.');
  } catch (err) {
    console.error('Fatal minification error:', err);
    process.exit(1);
  }
  console.timeEnd('Minification Time');
}

main();
