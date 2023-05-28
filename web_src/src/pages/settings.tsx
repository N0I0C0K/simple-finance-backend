import { useColorScheme } from '@mui/joy/styles'
import Button from '@mui/joy/Button'
import { Stack, Typography } from '@mui/joy'
import LightModeIcon from '@mui/icons-material/LightMode'
import DarkModeIcon from '@mui/icons-material/DarkMode'

function ModeToggle() {
  const { mode, setMode } = useColorScheme()
  return (
    <Button
      variant='outlined'
      color='neutral'
      startDecorator={mode === 'dark' ? <LightModeIcon /> : <DarkModeIcon />}
      onClick={() => setMode(mode === 'dark' ? 'light' : 'dark')}
    >
      {mode === 'dark' ? 'Turn light' : 'Turn dark'}
    </Button>
  )
}

export const Settings: React.FC = () => {
  return (
    <>
      <Stack gap={2}>
        <Typography level='h1'>Settings</Typography>
        <ModeToggle />
      </Stack>
    </>
  )
}
